#include <iostream>
#include <fstream>
#include <string>

#include "httplib.h"

using namespace std;

int main()
{
    httplib::Server server;

    server.set_default_headers({
        {"Access-Control-Allow-Origin", "*"},
        {"Access-Control-Allow-Methods", "GET, POST, OPTIONS"},
        {"Access-Control-Allow-Headers", "Content-Type"}
    });

    server.Get(
        "/",
        [](const httplib::Request& req,
           httplib::Response& res)
        {
            ifstream file("index.html");

            if (!file)
            {
                res.status = 404;
                res.set_content(
                    "index.html not found!",
                    "text/plain"
                );
                return;
            }

            string html(
                (istreambuf_iterator<char>(file)),
                istreambuf_iterator<char>()
            );

            res.set_content(html, "text/html");
        }
    );

    server.Post(
        "/check",
        [](const httplib::Request& req,
           httplib::Response& res)
        {
            string name;
            string ageText;
            string citizenship;

            if (req.has_param("name"))
                name = req.get_param_value("name");
            else
                name = "User";


            if (req.has_param("age"))
                ageText = req.get_param_value("age");
            else
            {
                res.set_content(
                    R"({"eligible":false,"message":"Age is required."})",
                    "application/json"
                );
                return;
            }


            if (req.has_param("citizenship"))
                citizenship = req.get_param_value("citizenship");
            else
            {
                res.set_content(
                    R"({"eligible":false,"message":"Country is required."})",
                    "application/json"
                );
                return;
            }


            int age;

            try
            {
                age = stoi(ageText);
            }
            catch (...)
            {
                res.set_content(
                    R"({"eligible":false,"message":"Please enter a valid age."})",
                    "application/json"
                );
                return;
            }


            // Invalid age check

            if (age <= 0 || age > 120)
            {
                string message =
                    "Please enter a realistic age between 1 and 120.";

                string response =
                    "{\"eligible\":false,"
                    "\"message\":\"" + message + "\"}";

                res.set_content(
                    response,
                    "application/json"
                );

                return;
            }


            // Voting eligibility check

            if (age >= 18 && citizenship == "Indian")
            {
                string message =
                    "Congratulations " + name +
                    "! You are eligible to vote.";

                string response =
                    "{\"eligible\":true,"
                    "\"message\":\"" + message + "\"}";

                res.set_content(
                    response,
                    "application/json"
                );
            }

            else
            {
                string reason;

                if (age < 18 && citizenship != "Indian")
                {
                    reason =
                        "You must be at least 18 years old and only Indian citizens are eligible to vote.";
                }

                else if (age < 18)
                {
                    reason =
                        "You must be at least 18 years old.";
                }

                else
                {
                    reason =
                        "Only Indian citizens are eligible to vote.";
                }


                string message =
                    name +
                    ", you are not eligible to vote. " +
                    reason;


                string response =
                    "{\"eligible\":false,"
                    "\"message\":\"" + message + "\"}";


                res.set_content(
                    response,
                    "application/json"
                );
            }
        }
    );


    cout << endl;
    cout << "========================================" << endl;
    cout << "       VOTING ELIGIBILITY CHECKER" << endl;
    cout << "           C++ BACKEND SERVER" << endl;
    cout << "========================================" << endl;
    cout << endl;

    cout << "Server started successfully!" << endl;
    cout << endl;

    cout << "Open webpage at:" << endl;
    cout << "http://localhost:9090" << endl;
    cout << endl;

    cout << "Press Ctrl + C to stop the server." << endl;
    cout << endl;


    server.listen("0.0.0.0", 9090);

    return 0;
}