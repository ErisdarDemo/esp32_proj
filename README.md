# esp32_proj

ESP32 Template for new project creation.

Place project into _proj\\_ with _main\\_ renamed _core\\_ using external source directories (_lbr\\_)

# Procedure

1. Create a new project _'New'_ -> _'Espressif IDF Project'_ using the _hello_world_ template

* Preparing for USB Console printf() & FreeRTOS

* Rename as _'esp32_reloc_dirs'_ using main.c with compile & debug (#818B)

2. Rename _main\\_ source directory

# 1 - Rename _main\\_ Directory

The build system provides special treatment to the main component. It is a component that gets automatically added to the build provided that it is in the expected location, PROJECT_DIR/main. All other components in the build are also added as its dependencies, saving the user from hunting down dependencies and providing a build that works right out of the box. Renaming the main component causes the loss of these behind-the-scenes heavy lifting, requiring the user to specify the location of the newly renamed component and manually specify its dependencies. Specifically, the steps to renaming main are as follows:

1. Rename main directory (e.g. _'src\\'_)

2. Set EXTRA_COMPONENT_DIRS in the project CMakeLists.txt to include the renamed main directory

	a. Locate the Project CMakeLists.txt at the root
	
	b. create(set) or append(list) to EXTRA_COMPONENT_DIRS
```	
	 set(EXTRA_COMPONENT_DIRS ${CMAKE_CURRENT_LIST_DIR}/src)
```

3. Specify the dependencies in the renamed component's CMakeLists.txt file via REQUIRES or PRIV_REQUIRES arguments on component registration (iff asked)

# 2 - Renaming _build\\_ Directory

Espressif uses build\ as a default and is not described well how to change this on a project-specific basis

## Normal Procedure

```
	cmake -B your_target_build_dir_name
```

Or

```
	set(CMAKE_BINARY_DIR ${CMAKE_BINARY_DIR}/../your_target_build_dir_name)
```	

But even here ESP-IDF still leaves a build dir, no matter what

## Mitigation

Compile project as normal with build\ at root, including build\ in your commit history

## Result

1. Hide build\ from the IDE (Project Explorer -> Filters and Customization -> User Filters -> New) & File Explorer

2. Clean then delete build\ for recompiles

3. Leave build\ alone! This is how ESP-IDF works

# 3 - Add _lbr\\_ Directory

1. Create new directory source & interface files for use

2. Add a CMakeLists.txt to this dir using the src\CMakeLists.txt for reference

3. Add reference to this dir in root CMakeLists.txt 'set(EXTRA_COMPONENT_DIRS ...)'

## How to use example

Follow detailed instructions provided specifically for this example.

Select the instructions depending on Espressif chip installed on your development board:

- [ESP32 Getting Started Guide](https://docs.espressif.com/projects/esp-idf/en/stable/get-started/index.html)
- [ESP32-S2 Getting Started Guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s2/get-started/index.html)

## Example folder contents

The project **hello_world** contains one source file in C language [hello_world_main.c](main/hello_world_main.c). The file is located in folder [main](main).

ESP-IDF projects are built using CMake. The project build configuration is contained in `CMakeLists.txt` files that provide set of directives and instructions describing the project's source files and targets (executable, library, or both).

Below is short explanation of remaining files in the project folder.

```
├── CMakeLists.txt
├── pytest_hello_world.py      Python script used for automated testing
├── main
│   ├── CMakeLists.txt
│   └── hello_world_main.c
└── README.md                  This is the file you are currently reading
```

For more information on structure and contents of ESP-IDF projects, please refer to Section [Build System](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/build-system.html) of the ESP-IDF Programming Guide.

## Troubleshooting

* Program upload failure

    * Hardware connection is not correct: run `idf.py -p PORT monitor`, and reboot your board to see if there are any output logs.
    * The baud rate for downloading is too high: lower your baud rate in the `menuconfig` menu, and try again.

## Technical support and feedback

Please use the following feedback channels:

* For technical queries, go to the [esp32.com](https://esp32.com/) forum
* For a feature request or bug report, create a [GitHub issue](https://github.com/espressif/esp-idf/issues)