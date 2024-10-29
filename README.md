# APT Game Project
APT is a dynamic, interactive game developed using C++ and the Qt framework. The game combines strategic movement, attack mechanisms, and various behavior models to create a unique gaming experience.

## Full Documentation - https://nicogutz.github.io


* [Home](https://nicogutz.github.io)   
    * [Architecture](docs/ARCH.md)
    * [General Notes](docs/NOTES.md)


## Class Structure    
(Most important classes marked with *)

```{cpp}
Project
├── controller
│   └── GameController*
├── view
│   ├── renderer
│   │   ├── Renderer*
│   │   ├── SpriteRenderer
│   │   ├── TextRenderer
│   │   └── ColorRenderer
│   ├── GamePixmapItem
│   ├── GameView*
│   └── GameWindow*
├── model
│   ├── behaviors
│   │   ├── concrete*
│   │   │   ├── attack
│   │   │   │   ├── CounterAttackBehavior
│   │   │   │   └── GenericAttackBehavior
│   │   │   ├── healing
│   │   │   ├── health
│   │   │   │   ├── GenericHealingBehavior
│   │   │   │   ├── GenericHealthBehavior
│   │   │   │   └── PoisonOnKilledBehavior
│   │   │   ├── movement
│   │   │   │   ├── GenericMoveBehavior
│   │   │   │   ├── GenericWalkableBehavior
│   │   │   │   ├── HealOnStepBehavior
│   │   │   │   ├── NewLevelOnStep
│   │   │   │   ├── ObstacleBehavior
│   │   │   │   ├── PoisonOnKilledBehavior
│   │   │   │   └── RandomMovementBehavior
│   │   │   ├── poison
│   │   │   │   ├── GenericPoisonableBehavior
│   │   │   │   └── GenericPoisoningBehavior
│   │   ├── Attack
│   │   ├── Behavior
│   │   ├── Health
│   │   ├── Movement
│   │   └── Poison
│   ├── noise
│   │   └── PerlinNoise
│   ├── GameObject*
│   ├── GameObjectModel*
│   ├── GameObjectSettings
│   └── ObjectModelFactory
├── Node
```

## Contributors

    • Nicolas Gutrierrez: Implemented the model and the behaviors (GameObject, GameObjectModel, GamePixmapItem, publicEnums, GameObjectSettings, + all the behaviors)

    • Nur Alda-Onggar: Implemented the views and the renderers (GameView, Renderer, TextRenderer, ColorRenderer, SpriteRenderer)

    • Salma Loukili: Implemented the controller and the UI (ModelFactory, GameController, GameWindow, GameWindow.ui, Node.h)
    
## Game Features Overview
### Subtask A
- [x] **Task 1: World Visualization**
  - Each tile has a fixed dimension in pixels and is colored based on its value.
  - Instead of using color, each tile can have an associated image.
  - The world is visualized as a scaled version of the image used to create it, zooming.

- [x] **Task 2: Object Visualization**
  - Protagonist, enemies, and health packs have specific visualizations matching tile dimensions.
  - Different enemy types have distinct visualizations; defeated enemies should be visually distinct.
  - Enemies gradually poison (random time) while poisoning the surrounding area, animated visually.

- [x] **Task 3: Actions Visualization**
  - Most actions (attack, health pack use, poison, movement, death) are animated.

- [x] **Task 4: Protagonist Control**
  - Control the protagonist with simple movements (up, right, down, left).
  - **Attack can be controlled (extra).** 

- [x] **Task 5: Autoplay Path Visualization**
  - Can visualize the path followed by the protagonist during autoplay.

- [x] **Task 7: Extensibility**
  - The architecture of the game is fully extensible. 

### Subtask B
- [x] **Task 1: ASCII Art-like World Visualization**
  - The world has an ASCII art-like, string-based visualization.

- [x] **Task 2: Integrated Text Visualization in UI**
  - The text visualization is integrated into the UI.
  - The user can easily switch between 'Graphical View' and 'Text View'.

- [x] **Task 3: Protagonist Actions in Text Mode**
  - Protagonist actions are visualized in text mode.
  - **Enemy actions are visualized in text mode (extra).**

- [x] **Task 4: Text-based Protagonist Interaction**
  - Interaction with the protagonist is done through text commands rather than mouse clicks.
  - Commands are extensible.
  - ~~Commands should be understood as soon as they are unique.~~

### Subtask C
_We took a different approach in our game since the worlds are procedural. Adding an image on top of a procedural world did not make much sense. The Sprite View makes a world using tile sprites based on the energy of the tiles. They look like different biomes (eventually it looks like a fever dream). The architecture pretty much completely separates the data layer from the visualization layer._

- [X] **Task 1: Separation of Concerns**
  - The visualization can differ from the data layer, allowing for varied appearances based on tile values.

- [ ] **Task 2: Image as Top Layer**
  - ~~Utilize a different image as a top layer, with tiles "behind" it forming a data layer with information.~~
  - ~~Entering specific areas like the door of a house triggers the loading of a new map.~~
  - ~~Maintain correlation between the top layer and the data layer when zooming in, ensuring the solution can handle this effectively.~~

### Subtask C

- [x] **Task 1: Special Tiles and Map Loading**
  - The protagonist can move to special tiles, triggering the loading of a different map.
  - Loading new worlds should be efficient and occur in a short time.

- [x] **Task 2: Map Caching for Optimization**
  - Implement a map caching mechanism to optimize memory management.

- [ ] **Task 3: Linking Various Levels**

_We make all the levels so there is no need for the user to create them._
  - ~~Devise a convenient method for users to create levels and indicate connections between them.~~

### Bonus Features
- **Gameplay**
  - Added Health to enemies and the protagonist, protagonist gains energy by killing enemies.
  - Enemies counter attack.
  - Pause / Resume / Restart / Quit Game
  - Timer
- **Visualizations**
  - Three separate visualizations for the world (Sprite, Text, Color).
  - All of them have generic animations that work regardless of the underlying Pixmap. 
  - Color visualizations have only simple shapes and change their color as health/energy goes down.
  - Seamless switch between the three at any point in the game.
  - Animations of scale, overlays, opacity, tint, using QParallelAnimation.

- **World Generation**
  - Infinite procedural world generation using Perlin Noise.
  - Variable game size, increasing energy of tiles as levels increase.

- **Autoplay**
  - User can decide to go to a location with the Autorouter or set the Autoplayer (-1, -1) to go until it dies.
  - Autoplay goes for health packs when it runs out of health or is poisoned. Kills enemies when low on health.
  - Can start at any point of the game.
 
## Weekly Progress Log

    • Week 6-11: Architecture of the game and UML
    • Week 10-12: Basic implementations and experimenting with QT
    • Week 13: Basic world grid and window implemented + model and basic behaviors implemented
    • Christmas Holiday: 
        - Day 1-3: Basic Integration of the world library with the model and the visualizations
        - Day 4-6: Finalized behaviors and integration to the window, integration of pathfinder, basic enemies
        - Day 6-8: Implementation of doorways and levels, integration of moving and poison enemies, finalized controller and wnidow, improved UI 
        - Day 8-10: Implementation of animations, improving the views, UML, Readme, Refactoring and documenting code



