#include <dpp/dpp.h>

#include <fstream>
#include <nlohmann/json.hpp>

void edit_webhook_from_json(dpp::cluster& bot, dpp::webhook& wh, const std::string& filename) {
    nlohmann::json data;
    std::ifstream file(filename);
    if (file.is_open()) {
        file >> data;
        file.close();

        // Required
        try {
            wh.id = std::stoull(data["id"].get<std::string>());
            wh.token = data["token"].get<std::string>();
            wh.name = data["name"].get<std::string>();
            wh.channel_id = std::stoull(data["channel_id"].get<std::string>());
        } catch (nlohmann::json::exception e) {
            std::cerr << "JSON parsing exception: " << e.what() << std::endl;
        }

        // Optional
        if (data.contains("guild_id") && !data["guild_id"].is_null()) {
            wh.guild_id = std::stoull(data["guild_id"].get<std::string>());
        }

        if (data.contains("avatar") && !data["avatar"].is_null()) {
            wh.avatar = data["avatar"].get<std::string>();
        }

        bot.edit_webhook_with_token(
            wh,
            [&wh, &bot](const dpp::confirmation_callback_t& callback) {
                if (callback.is_error()) {
                    std::cerr << "Error editing webhook: " << callback.get_error().message << std::endl;
                } else {
                    std::cout << "Webhook edited successfully!" << std::endl;

                    std::cout << "Webhook ID: " << wh.id << std::endl;
                    std::cout << "Webhook Token: " << wh.token << std::endl;
                    std::cout << "Webhook Name: " << wh.name << std::endl;
                    std::cout << "Channel ID: " << wh.channel_id << std::endl;
                }
            });

    } else {
        std::cerr << "Could not open file for reading!" << std::endl;
    }
}

int main() {
    std::string BOT_TOKEN;
    std::string WEBHOOK_URL;
    std::string JSON_FILENAME;

    // Reading env variables
    nlohmann::json ENV;
    std::ifstream ENV_file("./env.json");
    if (ENV_file.is_open()) {
        ENV_file >> ENV;
        ENV_file.close();

        BOT_TOKEN = ENV["BOT_TOKEN"];
        WEBHOOK_URL = ENV["WEBHOOK_URL"];
        JSON_FILENAME = ENV["JSON_FILENAME"];
    } else {
        std::cerr << "Could not open 'env.json' file for reading!" << std::endl;
    }

    dpp::cluster bot(BOT_TOKEN);

    dpp::webhook wh(WEBHOOK_URL);

    bot.on_log(dpp::utility::cout_logger());

    // Manual REST call to get webhook data
    bot.request(
        WEBHOOK_URL,
        dpp::m_get,
        [&bot, &wh, &JSON_FILENAME](const dpp::http_request_completion_t& http_response) {
            if (http_response.status == 200) {
                nlohmann::json response_data = nlohmann::json::parse(http_response.body);

                // Required
                try {
                    wh.id = std::stoull(response_data["id"].get<std::string>());
                    wh.token = response_data["token"].get<std::string>();
                    wh.name = response_data["name"].get<std::string>();
                    wh.channel_id = std::stoull(response_data["channel_id"].get<std::string>());
                } catch (nlohmann::json::exception e) {
                    std::cerr << "JSON parsing exception: " << e.what() << std::endl;
                }

                // Optional
                if (response_data.contains("guild_id") && !response_data["guild_id"].is_null()) {
                    wh.guild_id = std::stoull(response_data["guild_id"].get<std::string>());
                }

                if (response_data.contains("avatar") && !response_data["avatar"].is_null()) {
                    wh.avatar = response_data["avatar"].get<std::string>();
                }

                std::cout << "Webhook data loaded successfully!" << std::endl;

                edit_webhook_from_json(bot, wh, JSON_FILENAME);

            } else {
                std::cerr << "Error fetching webhook: " << http_response.status << std::endl;
            }
        });

    bot.start();

    return 0;
}