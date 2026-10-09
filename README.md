# Survival War: Pixel

A top-down 2D survival action game built in C++ with [SplashKit](https://splashkit.io). Dodge and fight off waves of enemies with a melee sword swing and a ranged bullet attack, and try to survive as long as possible.

## Controls

| Input | Action |
| --- | --- |
| `W` `A` `S` `D` / Arrow keys | Move the player |
| `Space` | Melee sword swing (damages any enemy inside the hit box) |
| Right mouse button | Fire a bullet toward the cursor |
| `Esc` (on the Game Over screen) | Quit |

The player starts with 3 hearts. Touching an enemy, or getting hit by one of their attacks, costs a heart. At 0 hearts the death animation plays, the screen fades out, and "GAME OVER" is shown. Enemies chase and melee-attack the player once within detection range (with a hit-frame check and an attack cooldown); outside that range they wander or rest autonomously, with each enemy's wander speed and "laziness" randomized individually so the group never behaves identically.

## Architecture

The game is built as a small object-oriented hierarchy rather than one procedural file. Design goals were encapsulation (every class owns and protects its own state), inheritance (shared behavior lives once, in a base class), and polymorphism (the game loop talks to `Player`/`Enemy` through a common interface, not type-specific code).

```
Entity (abstract)                SpriteAnimator
 ├─ position, size, active       encapsulates a sprite sheet's
 ├─ virtual draw() = 0           frame-stepping state (loop /
 └─ get_bounds()                 play-once-and-hold / play-once-
      │                          and-reset), used by composition
      ├─ Character (abstract)    inside Player and Enemy instead
      │   ├─ speed, attack       of duplicating timer bookkeeping
      │   └─ virtual is_alive() = 0
      │        ├─ Player
      │        └─ Enemy
      └─ Bullet
```

- **`Entity`** — the common base for anything that occupies space and can be drawn. Pure virtual `draw()` gives every subclass its own rendering while the game loop stays type-agnostic.
- **`Character`** — adds movement speed and attack strength on top of `Entity`. `is_alive()` is pure virtual because "alive" means something different for each subclass: `Player` tracks hearts, `Enemy` tracks a health pool.
- **`Player`** / **`Enemy`** — concrete `Character` subclasses. `Enemy` also owns its AI (chase, melee, wander, rest) and a static factory, `Enemy::spawn_wave(...)`, that places a wave of enemies at valid random positions away from the player and each other.
- **`Bullet`** — a lightweight `Entity` subclass: fires toward a target, moves, and deactivates off-screen or on impact.
- **`SpriteAnimator`** — a composed helper (not inherited) that owns a sprite sheet and its frame-timer state. `Player` and `Enemy` each hold several instances of it (walk / attack / death), which replaced five separate copies of hand-rolled "timer++; if timer >= duration ..." frame-stepping code with one tested implementation.
- **`Collision`** — a namespace of pure, stateless hit-test functions (`bullet_hits_enemy`, `player_hits_enemy`) that only read entity state through public accessors.
- **`Game`** — owns all game state (`Player`, a list of enemies, a list of bullets, background/audio) and drives the loop: input → AI/physics update → combat resolution → render. `main()` is now three lines.

Enemies and bullets are stored as `std::vector<std::unique_ptr<Enemy>>` and `std::vector<Bullet>` — smart pointers and RAII manage their lifetime, so there's no manual memory management or custom container code anywhere in the project.

### Other implementation notes

- **Collision detection**: bullets vs. enemies use each sprite's full bounding box; player vs. enemy uses a smaller box scaled to 25% of the sprite and centered, which avoids false hits from transparent padding around the artwork.
- **Background scaling**: the background bitmap is scaled to cover the window at its original aspect ratio, then centered with a computed offset.
- **Bullet system**: direction is the normalized vector from the firing point to the cursor at the moment of firing; bullets deactivate off-screen or on hit, and inactive bullets are swept from the vector every frame.

## Running the game

1. Install the [SplashKit SDK](https://splashkit.io/installation/).
2. From the `1045Dproject` directory, compile all source files together:
   ```
   skm g++ *.cpp -o game
   ```
3. Run the resulting executable.

## Project structure

```
1045Dproject/
├── Entity.hpp / Entity.cpp              # Abstract base: position, size, draw()
├── Character.hpp / Character.cpp        # Entity subclass: speed, attack, is_alive()
├── Player.hpp / Player.cpp              # Character subclass: input, hearts, animations
├── Enemy.hpp / Enemy.cpp                # Character subclass: AI, wave spawning
├── Bullet.hpp / Bullet.cpp              # Entity subclass: projectile movement
├── SpriteAnimator.hpp / SpriteAnimator.cpp  # Reusable sprite-sheet animation
├── Collision.hpp / Collision.cpp        # Stateless hit-testing helpers
├── Game.hpp / Game.cpp                  # Owns game state and runs the main loop
├── main.cpp                             # Entry point
└── Resources/
    ├── sprites/                          # Character, enemy and background sprite sheets
    └── sounds/                           # Background music and sound effects
```
