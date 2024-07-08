add_library(assignment)

target_sources(assignment
PRIVATE
	image_processing_service.h
	robot_controller.cc
	robot_controller.h
	venipuncture_service.cc
	venipuncture_service.h
)

target_include_directories(assignment
PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(assignment_cli)

target_sources(assignment_cli
PRIVATE
	main.cc
	robot_controller_ui.cc
	robot_controller_ui.h
)

target_link_libraries(assignment_cli
PRIVATE
	assignment
)

