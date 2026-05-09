cmake_minimum_required(VERSION 3.5)
project(skidpad_bringup)

find_package(ament_cmake REQUIRED)

# 安装启动文件
install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME}
)

ament_package()