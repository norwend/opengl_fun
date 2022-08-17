#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


enum CameraMovement { FORWARD, BACKWARD, UP, DOWN, LEFT, RIGHT };

class Camera {
    glm::vec3 position_;
    glm::vec3 start_pos_;
    glm::vec3 front_;
    glm::vec3 up_;

    float lastX, lastY;
    
    float yaw_ = -90.0f;
    float pitch_ = 0.0f;
    float speed_ = 0.1f;
    float zoom_ = 45.0f;
    constexpr static float SENSITIVITY = 0.15f;
    constexpr static float camera_speed = 5.0f;
    bool firstMouse = true;

public:
    Camera(uint32_t scr_width, uint32_t src_height, glm::vec3 pos, glm::vec3 front) : position_(pos), front_(front) {
	lastX = (float)scr_width / 2;
	lastY = (float)src_height / 2;
	up_ = glm::vec3(0.0, 1.0, 0.0);
	start_pos_ = position_;
    }

    glm::mat4 get_view() {
	// return glm::lookAt(position_, position_ + front_, up_);
	return glm::lookAt(position_, position_ + front_, up_);
    }

    float get_zoom() const {
	return zoom_;
    }

    void mouse_callback(double xpos, double ypos) {
	if (firstMouse) {
	    lastX = xpos;
	    lastY = ypos;
	    firstMouse = false;
	}
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;
	xoffset *= SENSITIVITY;
	yoffset *= SENSITIVITY;
	yaw_ += xoffset;
	pitch_ += yoffset;
	if(pitch_ > 89.0f)
	    pitch_ = 89.0f;
	if(pitch_ < -89.0f)
	    pitch_ = -89.0f;
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw_));
	direction.y = sin(glm::radians(pitch_));
	direction.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
	front_ = glm::normalize(direction);
    }

    void scroll_callback(double xoffset, double yoffset) {
	zoom_ -= (float)yoffset;
	if (zoom_ < 1.0f)
	    zoom_ = 1.0f;
	if (zoom_ > 45.0f)
	    zoom_ = 45.0f;
    }

    void move(CameraMovement dir, float delta_time) {
	switch (dir) {
	case FORWARD:
	    position_ += camera_speed * front_ * delta_time;
	    break;
	case BACKWARD:
	    position_ -= camera_speed * front_ * delta_time;
	    break;
	case UP:
	    position_ += up_ * camera_speed * delta_time;
	    break;
	case DOWN:
	    position_ -= up_ * camera_speed * delta_time;
	    break;
	case LEFT:
	    position_ += glm::normalize(glm::cross(up_, front_)) * camera_speed * delta_time;
	    break;
	case RIGHT:
	    position_ -= glm::normalize(glm::cross(up_, front_)) * camera_speed * delta_time;
	    break;
	default:
	    break;
	}
	position_ = glm::vec3(position_.x, start_pos_.y, position_.z);
    }
};

