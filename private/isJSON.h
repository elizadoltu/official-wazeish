#include <jansson.h>
#include <stdio.h>

bool isJSON(const char *data) {

    // Parse the data as JSON
    json_t *root = json_loads(data, 0, NULL);

    // Check if parsing was successful 
    bool isValidJSON = (root != NULL);

    // Free the JSON object
    if (root != NULL) {
        json_decref(root);
    }

    return isValidJSON;
}