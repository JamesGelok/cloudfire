# ⬜ CloudFire 🟧

![CloudFire Demo](./demo.gif)

## Description

**⬜ CloudFire 🟧** is an immersive 3D platformer where you embody a fiery elemental navigating through a series of floating clouds. As a vibrant orange cube, your mission is to skillfully platform across white clouds, ascending to reach the highest cloud in the sky. Test your agility and precision as you maneuver through procedurally-generated environments, overcoming obstacles and mastering the art of aerial navigation in a visually captivating setting.

### Gameplay Features

- **Play as a Fire Elemental:** Control a lively orange cube representing a fire elemental with unique abilities (like _jumping_ and _MOVING AROUND_ and _*TURNING*_).
- **Platforming Challenges:** Traverse through _meticulously designed_ white cloud rectangles, each procedurally generated to provide unique challenges and layouts every time you play.
- **Ascend to New Heights:** Aim to reach the pinnacle of the cloudscape by skillfully jumping and moving between clouds, your reward for mastering the art of aerial navigation is the breathtaking view from the highest cloud of the sky.
- **Dynamic Physics:** Experience a handful of realistic physics interactions, including gravity and collision detection, enhancing the gameplay experience.
- **Responsive Controls:** Enjoy smooth and intuitive controls that allow for precise movement and rotation.
- **Visual Effects:** Appreciate visually appealing graphics with dynamic lighting and colors that bring the cloudscape to life.

### Controls

- **Turn Left/Right:** Use the **Left** and **Right Arrow Keys** to rotate your elemental, allowing for precise navigation.
- **Move Forward/Backward:** Use the **Up** and **Down Arrow Keys** to move your elemental forward and backward across the cloud platforms.
- **Jump:** Press the **Spacebar** to make your elemental jump onto higher clouds, avoiding pitfalls and reaching new heights.

## Architecture Overview

**⬜ CloudFire 🟧** is built on a robust and flexible architecture centered around the **Entity-Component-System (ECS)** paradigm. This design ensures modularity, scalability, and efficient management of game entities, facilitating future enhancements and aspirations to transition this project into a fully-fledged general-purpose game engine one day.

### Core Components

- **Entity-Component-System (ECS):**

An **[Entity-Component-System (ECS)](https://en.wikipedia.org/wiki/Entity_component_system)** is a game architecture pattern that organizes entities, their data (components), and the logic (systems) that operates on them, promoting modularity and flexibility. The game loop acts like a heartbeat, driving systems to update entities in an efficient and organized manner.

- **Entities:** Represent game objects (e.g., player, platforms) identified by unique IDs.
- **Components:** Hold data relevant to entities (e.g., [`Position`](https://github.com/JamesGelok/cloudfire/blob/master/src/components/Position.h), [`Velocity`](https://github.com/JamesGelok/cloudfire/blob/master/src/components/Velocity.h), [`Renderable`](https://github.com/JamesGelok/cloudfire/blob/master/src/components/Renderable.h), [etc](https://github.com/JamesGelok/cloudfire/tree/master/src/components)).
- **Systems:** Contain the logic and behavior, operating on entities that possess specific components (e.g., [`RenderSystem`](https://github.com/JamesGelok/cloudfire/blob/master/src/systems/RenderSystem.h), [`PhysicsSystem`](https://github.com/JamesGelok/cloudfire/blob/master/src/systems/PhysicsSystem.h), [etc](https://github.com/JamesGelok/cloudfire/tree/master/src/systems)).

There are also **Managers** that oversee game logic and data management, such as the **EntityManager** and **ComponentManager**, which handle entity creation/destruction and component storage/retrieval, respectively.

### Some key features of the architecture include:

- **Component Masking with IDs:**

  - Uses a bitmask where each bit represents the presence of a specific component in an entity, allowing systems to quickly determine which entities to process based on their assigned components.

- **Rendering System:**

  - Built using OpenGL, the rendering system manages shaders, projection matrices, and lighting configurations to render 3D models. [RenderSystem](https://github.com/JamesGelok/cloudfire/blob/master/src/systems/RenderSystem.cpp)
  - **Shaders:** Simple custom vertex and fragment shaders handle transformations and lighting. [Shaders](https://github.com/JamesGelok/cloudfire/tree/master/shaders)

- **Model Loading:**
  - Utilizes **Assimp** to load 3D models, with a custom [`ModelLoader`](https://github.com/JamesGelok/cloudfire/blob/master/src/components/ModelLoader.cpp) made to eventually handle model with textures.

### Managers

The ECS framework in **CloudFire 🟧** ensures a clear separation between data and behavior:

1. **[EntityManager](https://github.com/JamesGelok/cloudfire/blob/master/src/core/Entity.h):** Manages the creation and destruction of entities, maintaining a pool of available entity IDs and tracking component associations using bitmasking.

2. **[ComponentManager](https://github.com/JamesGelok/cloudfire/blob/master/src/managers/ComponentManager.h):** Handles the storage and retrieval of components, utilizing templates for efficient access and modification of different component types.

## Building and Running the Game

### Prerequisites

Ensure you have the following dependencies installed:

- **C++17 Compiler:** Compatible with C++17 standards (e.g., `g++`, `clang++`).
- **GLFW:** For window creation and input handling.
- **GLAD:** For managing OpenGL function pointers.
- **Assimp:** For loading 3D models.
- **stb Libraries:** For image loading (textures).
- **GLM:** For mathematical operations (vectors, matrices, quaternions).

### Installation of Dependencies (macOS)

Since the project has been primarily tested on macOS, here are the installation steps using [Homebrew](https://brew.sh/):

```bash
# Install Homebrew if not already installed
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install dependencies
brew install glfw assimp glm
```

### Compilation

The project currently uses a `Makefile` for compilation. To compile the project:

1. **Navigate to the Project Directory:**

   ```bash
   cd path/to/CloudFire
   ```

2. **Run Make:**

   ```bash
   make
   ```

   This command compiles the source files and links them to create the executable.

### Running the Game

After successful compilation, run the executable:

```bash
./main
```

### Current Platform Support

**⬜ CloudFire 🟧** has been thoroughly tested on **macOS**. While it should function on other platforms with minor adjustments, comprehensive testing on Windows and Linux is pending.

### Future Platform Plans

To enhance cross-platform compatibility and streamline the build process, we plan to transition from `Makefile` to **CMake**. This will facilitate easier compilation across different operating systems and environments.

## Testing and Quality Assurance

### Current Testing

- **Platform:** The game has been thoroughly tested on **macOS**, ensuring stability and performance on this platform.
- **Functionality:** Core gameplay mechanics, rendering, and input systems have been validated to work seamlessly.
- **Performance:** The game maintains a high frame rate and smooth gameplay experience.

### Planned Testing

- **Cross-Platform Testing:** Extend testing to **Windows** and **Linux** to ensure compatibility and address platform-specific issues.
- **Automated Testing:** Implement automated tests for critical systems to maintain code quality and detect regressions early.
- **Performance Profiling:** Continuously profile the game to identify and optimize performance bottlenecks, ensuring smooth gameplay even as the engine scales.
- **User Testing:** Conduct user testing sessions to gather feedback on gameplay mechanics, controls, and overall user experience, guiding iterative improvements.

## Future Aspirations

While **CloudFire 🟧** currently serves as a functional 3D platformer, we aspire to evolve it into a comprehensive, general-purpose game engine. Future enhancements include:

- **Cross-Platform Support:** Migrating the build system from `Makefile` to **CMake** for improved cross-platform support and easier build configurations, and testing on Windows and Linux.
- **Enhanced ECS Framework:** Expanding the Entity-Component-System to support more complex game mechanics and systems.
- **Advanced Rendering Features:** Integrating sophisticated shaders, shadow mapping, and particle systems to elevate visual fidelity.
- **Networking and Multiplayer:** Introducing multiplayer capabilities to allow real-time player interactions.
- **Project Separation:** Splitting the project into two distinct repositories—one for the game and another for the underlying game engine—to promote reusability and independent development.
- **Advanced Physics Integration:** Incorporating more sophisticated physics simulations to enhance realism and interactivity.

For detailed plans and to contribute towards these aspirations, please refer to our [GitHub Issues](https://github.com/JamesGelok/cloudfire/issues).

## Contributing

We welcome contributions from the community to help enhance **CloudFire 🟧**. Whether you're looking to report a bug, suggest a feature, or submit a pull request, your input is invaluable to the growth and improvement of **CloudFire 🟧**. Please follow these guidelines to ensure a smooth collaboration process.

### How to Contribute

1. **Fork the Repository:**

   - Click the **Fork** button at the top-right corner of the repository page to create your personal fork.

2. **Clone Your Fork:**

   ```bash
   git clone https://github.com/JamesGelok/cloudfire.git
   cd cloudfire
   ```

3. **Create a Feature Branch:**

   - Use descriptive names for your branches to indicate the purpose of your changes.

   ```bash
   git checkout -b feature/YourFeatureName
   ```

4. **Make Your Changes:**

   - Implement your feature, fix bugs, or improve documentation.
   - Ensure your code adheres to the project's coding standards and practices.

5. **Commit Your Changes:**

   ```bash
   git commit -m "Add [description of your feature or fix]"
   ```

6. **Push to Your Fork:**

   ```bash
   git push origin feature/YourFeatureName
   ```

7. **Open a Pull Request:**
   - Navigate to the original repository on GitHub.
   - Click the **Compare & pull request** button for your branch.
   - Provide a clear and concise description of your changes, including the motivation and any relevant context.

## Code of Conduct

Please adhere to the [Code of Conduct](https://github.com/JamesGelok/cloudfire/blob/master/CODE_OF_CONDUCT.md) to ensure a welcoming and respectful environment for all contributors.

## License

This project is open-source and available under the [MIT License](https://github.com/JamesGelok/cloudfire/blob/master/LICENSE). Feel free to use, modify, and distribute the code in your own projects. For more details, please refer to the [LICENSE](https://github.com/JamesGelok/cloudfire/blob/master/LICENSE) file.

## Acknowledgements

- **[Assimp](https://github.com/assimp/assimp):** Simplifies 3D model loading and processing.
- **[GLFW](https://github.com/glfw/glfw) & [GLAD](https://github.com/Dav1dde/glad):** Provide essential OpenGL functionalities and manage context creation and input handling.
- **[stb Libraries](https://github.com/nothings/stb):** Facilitate efficient image loading and other utilities.
- **[GLM](https://github.com/g-truc/glm):** Offers comprehensive mathematical operations, including vectors, matrices, and quaternions.
- **[OpenGL](https://www.opengl.org/):** The graphics API that powers the rendering engine, enabling cross-platform graphics rendering.

---

**Happy Gaming!** 🚀🔥
