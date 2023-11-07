#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <jansson.h>
#define API_KEY "cbc36000-5818-4482-862e-354b38a5a174"

size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    char **response_ptr = (char **)userp;

    *response_ptr = (char *)malloc(realsize + 1);
    if (*response_ptr) {
        memcpy(*response_ptr, contents, realsize);
        (*response_ptr)[realsize] = 0; 
    }

    return realsize;
}

void get_crypto_price(const char *symbol) {
    CURL *curl;
    CURLcode res;

    char url[200];
    snprintf(url, sizeof(url), "https://pro-api.coinmarketcap.com/v1/cryptocurrency/quotes/latest?symbol=%s&convert=USD&CMC_PRO_API_KEY=%s", symbol, API_KEY);

    char *response = NULL;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        res = curl_easy_perform(curl);

        if (res == CURLE_OK) {
            json_t *root;
            json_error_t error;
            root = json_loads(response, 0, &error);
            if (root) {
                json_t *quote = json_object_get(root, "data");
                json_t *price = json_object_get(quote, symbol);
                json_t *usd = json_object_get(price, "quote");
                json_t *usd_price = json_object_get(usd, "USD");
                double price_value = json_real_value(json_object_get(usd_price, "price"));
                printf("Current price of %s: $%.2f\n", symbol, price_value);
                json_decref(root);
            } else {
                fprintf(stderr, "Failed to parse JSON response: %s\n", error.text);
            }
        } else {
            fprintf(stderr, "Failed to fetch data: %s\n", curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);

        if (response) {
            free(response);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <crypto_symbol>\n", argv[0]);
        return 1;
    }

    get_crypto_price(argv[1]);

    return 0;
}