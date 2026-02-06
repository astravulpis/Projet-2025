#include <raylib.h>
#include "../shared.h"

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
    KeyboardKey rotLeft;  //!< Down movement key
    KeyboardKey rotRight;  //!< Down movement key
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
    [LAYOUT_QWERTY] = (keymap){KEY_A, KEY_D, KEY_W, KEY_S, KEY_Q, KEY_E},
    [LAYOUT_AZERTY] = (keymap){KEY_Q, KEY_D, KEY_Z, KEY_S, KEY_A, KEY_E},
};

int main(int argc, char *argv[])
{
    float rot = 0.0f;
    Vector2 imgPos = {200, 200};
    float scale = 0.15f;
    int movementSpeed = 5;

    bool *azertyActive = flag_bool("azertySchema", false, "Use the azerty layout schema");
    keymap activeLayout = keyLayout[LAYOUT_QWERTY];

    flag_parse(argc, argv);

    if (*azertyActive) {
        activeLayout = keyLayout[LAYOUT_AZERTY];
    }


    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "ULTRAC00L - Window Display");
    SetTargetFPS(60);
    Texture2D logoC = LoadTexture("./assets/img/logoC.png");

    while (!WindowShouldClose()) {
        // Rotation
        if (IsKeyDown(activeLayout.rotLeft)) {
            rot = (rot >= 360) ? -360 : rot + 1;
        } else if (IsKeyDown(activeLayout.rotRight)) {
            rot = (rot <= -360) ? 360 : rot - 1;
        }

        // Vertical Movement
        if (IsKeyDown(activeLayout.down)) {
            imgPos.y = (imgPos.y+logoC.height*scale >= WINDOW_HEIGHT) ? WINDOW_HEIGHT - logoC.height*scale : imgPos.y + movementSpeed;
        } else if (IsKeyDown(activeLayout.up)) {
            imgPos.y = (imgPos.y <= 0) ? 0 : imgPos.y - movementSpeed;
        }

        // Horizontal Movement
        if (IsKeyDown(activeLayout.right)) {
            imgPos.x = (imgPos.x+logoC.width*scale >= WINDOW_WIDTH) ? WINDOW_WIDTH - logoC.width*scale : imgPos.x + movementSpeed;
        } else if (IsKeyDown(activeLayout.left)) {
            imgPos.x = (imgPos.x <= 0) ? 0 : imgPos.x - movementSpeed;
        }

        BeginDrawing(); { // Note: Les `{` et `}` ne sont pas obligatoires. Elles ne sont que pour un formattage plus comprehensible
                          //       de "qu'est ce qui se passe a cette endroit". Si elles vous genent, vous pouvez les enlever.
            ClearBackground(GetColor(0x6495EDFF));
            DrawTextureEx(logoC, imgPos, rot, scale, WHITE);
        } EndDrawing();

#if 0
        // Une simple indentation peut aussi fonctionner.
        BeginDrawing();
            ClearBackground(GetColor(0x6495EDFF));
        EndDrawing();
#endif // 0

#if 0
        // Ou juste laisser le tout sur la meme colonne.
        BeginDrawing();
        ClearBackground(GetColor(0x6495EDFF));
        EndDrawing();
#endif // 0
    }
    CloseWindow();
    return 0;
}
