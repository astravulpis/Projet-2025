#include "jsonParsing.h"

bool readJSON(player_t * player){
    FILE *fp = fopen("./Saves/player_save.json", "r");
    if (fp == NULL) {
        printf("Error: Unable to open the file.\n");
        return 1;
    }
    // read the file contents into a string
    char buffer[1024];
    size_t len = fread(buffer, 1, sizeof(buffer) - 1, fp);
    buffer[len] = '\0';
    fclose(fp);

    printf("now parsing in the json file \n");
    // parse the JSON data
    cJSON *json = cJSON_Parse(buffer);
    if (json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            printf("Error: %s\n", error_ptr);
        }
        return 1;
    }
    printf("now going to get the score value from the json file \n");
    // access the JSON data
    cJSON *score = cJSON_GetObjectItemCaseSensitive(json, "score");
    if (cJSON_IsNumber(score)) {
        printf("score in JSON: %f\n", score->valuedouble);
        player->score = (float)cJSON_GetNumberValue(score); 
    } else {
        printf("Warning: score field missing or invalid type in JSON.\n");
    }

    // delete the JSON object
    cJSON_Delete(json);
    return 1;
}

bool writeJSON(player_t * player){
    cJSON *json = cJSON_CreateObject();

    printf("player scoreat the end of the game: %f\n", player->score);
    // modify the JSON data
    cJSON_AddNumberToObject(json, "score", player->score);

    // convert the cJSON object to a JSON string
    char *json_str = cJSON_Print(json);

    printf("json_str contents: %s\n", json_str);

    // write the JSON string to the file
    FILE *fp = fopen("./Saves/player_save.json", "w");
    if (fp == NULL) {
        printf("Error: Unable to open the file.\n");
        return 1;
    }
    printf("%s\n", json_str);
    fputs(json_str, fp);
    fclose(fp);

    // free the JSON string and cJSON object
    cJSON_free(json_str);
    cJSON_Delete(json);

    return 0;
}