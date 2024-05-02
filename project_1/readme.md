# Project 1
Project 1 for the E-Agle TRT Software recruiting test.

## Abstract
This is a GUI application that simulates visualization of F1 telemetry data from a qualifying lap.  
There are multiple users, each one with a different access level to the application, and each one can access different tabs.

## Data management
At first, the application doesn't load any data, meaning that if a normal user logs in, no data will be present.
The data can be loaded by an Admin user first, then it will be available to other users for visualization (even after a logout).

### Loading data
The `Manage Data` tab contains a field to specify a driver to load.  
The driver data (if present) gets loaded from a file inside the `telemetry/` folder, with the name `<driver_identifier>.csv`.  
The driver identifier is an upper-case string of 3 characters, like `LEC`: the currently available drivers are `LEC`, `VER` and `HAM`.


## Tabs
The application is composed of 5 main tabs:
- **Info**: Displays some info about the current state, such as the current user and its permissions
- **Login**: Contains the login form, where the user can login with its credentials. Only visible if the user is not logged in.
- **Data Plot**: Contains the telemetry plots, if some data has been loaded.
- **Style**: Color pickers to change the plotting colors for each loaded driver.
- **Data Management**: Load new drivers for plotting, or remove the existing ones.

## Users
There are 3 access levels:

| Username     | Password   | Access Level   | Description                                                                                |
|--------------|------------|----------------|--------------------------------------------------------------------------------------------|
| `user`       | `password` | User (2)       | The normal user, can login and view the loaded telemetry data.                             |
| `maintainer` | `password` | Maintainer (1) | Same privileges as the User, and can modify the plotting style.                            |
| `admin`      | `password` | Admin (0)      | All privileges (user and maintainer privileges, and can manage the loaded telemetry data). |


## Getting started

### Prerequisites

- `git` and a [GitHub](https://github.com) account
- C/C++ toolchain, with CMake
- OpenGL
- GLEW
- GLFW3

For Debian / Ubuntu you can use:

```bash
sudo apt install build-essential cmake libglfw3-dev libglew-dev mesa-utils
```

### Building

You can build and run the application with the following commands:

```bash
mkdir -p build
cd build
cmake ..
make -j$(nproc)
cd ..
./bin/project1
```

This will build the executable that will be located in `./bin` directory.
