#include "../shared.h"
#include <raylib.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

/**
 * @struct movementKey_s
 * @brief Represents a physical key mapping for directional movement.
 */
typedef struct movementKey_s {
    KeyboardKey left;  //!< Left movement key
    KeyboardKey right; //!< Right movement key
    KeyboardKey up;    //!< Up movement key
    KeyboardKey down;  //!< Down movement key
} keymap;

/**
 * @enum LayoutType
 * @brief Identifiers for supported keyboard layouts.
 */
typedef enum layoutType_s {
    LAYOUT_QWERTY, //!< Standard mapping for US/UK keyboards.
    LAYOUT_AZERTY, //!< Standard mapping for French keyboards.
} LayoutType;

/**
 * @brief Physical key mapping presets.
 * Access via keySchema[LAYOUT_QWERTY], etc.
 */
static keymap keyLayout[] = {
    [LAYOUT_QWERTY] = (keymap){.left = KEY_A, .right = KEY_D, .up = KEY_SPACE, .down = KEY_LEFT_SHIFT},
    [LAYOUT_AZERTY] = (keymap){.left = KEY_Q, .right = KEY_D, .up = KEY_SPACE, .down = KEY_LEFT_SHIFT},
};

int main(int argc, char *argv[])
{
    float rot = 0.0f;
    Vector2 imgPos = {200, 200};
    float scale = 0.05f;
    float movementSpeed = 150.0f;
    Vector2 vel = {0};
    float gravity = 9.2f;
    float jumpForce = -4.5f;
    KeyboardKey lastDirection = KEY_NULL;
    float stunnedTimer = 0.0f;
    bool onGround = false;
    bool slammingDown = false;

    bool *azertyActive = flag_bool("azertySchema", false, "Use the azerty layout schema");
    keymap activeLayout = keyLayout[LAYOUT_QWERTY];

    flag_parse(argc, argv);

    if (*azertyActive) {
        activeLayout = keyLayout[LAYOUT_AZERTY];
    }

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "ULTRAC00L - Window Display");
    SetTargetFPS(144);
    Texture2D logoC = LoadTexture("./assets/img/logoC.png");

    size_t mark = temp_save();

    while (!WindowShouldClose()) {
        temp_rewind(mark);

        if (stunnedTimer > 0) {
            stunnedTimer = (stunnedTimer <= 0) ? 0 : stunnedTimer - 0.25f * GetFrameTime();
        }

        if (imgPos.y + logoC.height * scale >= WINDOW_HEIGHT) {
            onGround = true;
            imgPos.y = WINDOW_HEIGHT - logoC.height * scale;
            vel.y = 0;
            slammingDown = false;
        } else {
            onGround = false;
            stunnedTimer = 0.025;
        }

        if (IsKeyPressed(activeLayout.up) && onGround) {
            vel.y = jumpForce;
        }

        vel.y += gravity * GetFrameTime();
        imgPos.y += vel.y;
        if (slammingDown) {
            imgPos.y -= jumpForce * 1.45f;
        }

        // Vertical Movement
        if (IsKeyDown(activeLayout.down)) {
            if (!onGround) {
                slammingDown = true;
            } else {
                if (stunnedTimer <= 0.0f) {
                    if (lastDirection == KEY_NULL || lastDirection == activeLayout.right) {
                        imgPos.x = (imgPos.x + logoC.width * scale >= WINDOW_WIDTH) ? WINDOW_WIDTH - logoC.width * scale
                                                                                    : imgPos.x + (movementSpeed * 2) * GetFrameTime();

                    } else if (lastDirection == activeLayout.left) {
                        imgPos.x = (imgPos.x <= 0) ? 0 : imgPos.x - (movementSpeed * 2) * GetFrameTime();
                    }
                }
            }
        }

        // Horizontal Movement
        if (IsKeyDown(activeLayout.right)) {
            imgPos.x = (imgPos.x + logoC.width * scale >= WINDOW_WIDTH) ? WINDOW_WIDTH - logoC.width * scale
                                                                        : imgPos.x + movementSpeed * GetFrameTime();
            lastDirection = activeLayout.right;
        } else if (IsKeyDown(activeLayout.left)) {
            imgPos.x = (imgPos.x <= 0) ? 0 : imgPos.x - movementSpeed * GetFrameTime();
            lastDirection = activeLayout.left;
        }

        BeginDrawing();
        {
            ClearBackground(GetColor(0x181818FF));
            DrawTextureEx(logoC, imgPos, rot, scale, WHITE);
            DrawText(temp_sprintf("x: %.2f, y: %.2f", imgPos.x, imgPos.y), 10, 10, 18, WHITE);
            DrawText(temp_sprintf("stunned: %.2f", stunnedTimer), 10, 24, 18, WHITE);
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
