#ifndef CONTROLS_HPP
#define CONTROLS_HPP

void Scroll(GLFWwindow *window, double posX, double posY);
void computeMatricesFromInputsNew();
void computeMatricesFromInputs();
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();


#endif
