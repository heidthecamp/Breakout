
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <cstdlib>
#include <ctime>
#include <cmath>

float deg_to_rad(float angle)
{
    return angle * (3.14159f / 180.0f);
}

int main(int argc, char * argv[])
{
    const unsigned FPS = 60;
    const float START_DELAY = 3.0f;
    const float PADDLE_SPEED = 5.5f;
    const float BALL_SPEED = 8.0f;

    srand(time(nullptr));

    sf::RenderWindow window(
        sf::VideoMode(1024, 768),
        "Breakout"
    );

    sf::Vector2f windowSize(window.getSize());

    // window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(FPS);

    sf::Font font;
    font.loadFromFile("arcadeclassic.regular.ttf");

    sf::SoundBuffer soundPaddleHit;
    soundPaddleHit.loadFromFile("paddle.ogg");
    sf::SoundBuffer soundWallHit;
    soundWallHit.loadFromFile("wall.ogg");
    sf::SoundBuffer soundBrickHit;
    soundBrickHit.loadFromFile("bric.ogg");
    sf::SoundBuffer soundPowerup;
    soundPowerup.loadFromFile("powerup.ogg");
    sf::SoundBuffer soundDie;
    soundDie.loadFromFile("die.ogg");

    sf::Sound sound;
    sound.setVolume(5.0f);

    sf::Text startText("PRESS  ENTER  TO  START", font, 50);
    startText.setOrigin(startText.getLocalBounds().getSize() * 0.5f);
    startText.setPosition(windowSize * 0.5f);

    sf::Text readyText("READY", font, 50);
    readyText.setOrigin(readyText.getLocalBounds().getSize() * 0.5f);
    readyText.setPosition(window.getSize().x * 0.5f, (window.getSize().y * 0.5f) - 150);

    sf::Text setText("SET", font, 50);
    setText.setOrigin(setText.getLocalBounds().getSize() * 0.5f);
    setText.setPosition(window.getSize().x * 0.5f, (window.getSize().y * 0.5f) - 150);

    sf::RectangleShape paddle({100.0f, 20.0f});
    paddle.setFillColor(sf::Color::White);
    paddle.setOrigin(paddle.getSize().x * 0.5f, 0.0f);


    sf::CircleShape ball(10.0f);
    ball.setFillColor(sf::Color::White);
    ball.setOrigin(ball.getRadius(), ball.getRadius());

    sf::Vector2f ballSpeed;

    unsigned countDown = 0;
    bool playing = false;



    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Enter && !playing) {
                    playing = true;

                    paddle.setPosition(
                        window.getSize().x * 0.5f,
                        window.getSize().y - paddle.getSize().y - 20.0f
                    );

                    ball.setPosition(
                        window.getSize().x * 0.5f,
                        window.getSize().y * 0.5f
                    );

                    ballSpeed = {0.0f, 0.0f};

                    countDown = FPS * START_DELAY;
                }
            }
        }

        if (playing) {
            if (countDown > 0) {
                --countDown;

                if (countDown == 0) {
                    float angle = 45.0f + (float)(rand() % 90);
                    angle = deg_to_rad(angle);

                    ballSpeed.x = cosf(angle) * BALL_SPEED;
                    ballSpeed.y = sinf(angle) * BALL_SPEED;
                }
            }

            ball.move(ballSpeed);
            // if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            //     ball.move(0.0, -SPEED);
            // }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                paddle.move(-PADDLE_SPEED, 0.0f);
            }
            // if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            //     ball.move(0.0, SPEED);
            // }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                paddle.move(PADDLE_SPEED, 0.0);
            }

            if ((paddle.getPosition().x - (paddle.getSize().x * 0.5f)) < 0) {
                paddle.setPosition((paddle.getSize().x * 0.5f), paddle.getPosition().y);
            }

            if ((paddle.getPosition().x + (paddle.getSize().x * 0.5f)) > window.getSize().x) {
                paddle.setPosition((windowSize.x - (paddle.getSize().x * 0.5f)), paddle.getPosition().y);
            }
            
            const auto& ballBound = ball.getGlobalBounds();

            if (paddle.getGlobalBounds().intersects(ballBound)) {
                sound.setBuffer(soundPaddleHit);
                float dx = (ball.getPosition().x - paddle.getPosition().x);
                dx /= paddle.getSize().x * 0.5f;

                float angle = 270.0f + (dx * 45.0f);
                angle = deg_to_rad(angle);

                ballSpeed.x = cosf(angle) * BALL_SPEED;
                ballSpeed.y = sinf(angle) * BALL_SPEED;
                sound.play();
            }

            if ((ballBound.left < 0.0f) || ((ballBound.left + ballBound.width) > windowSize.x)) {
                sound.setBuffer(soundWallHit);
                ballSpeed.x *= -1.0f;
                sound.play();
            }

            if (ballBound.top < 0.0f) {
                sound.setBuffer(soundWallHit);
                ballSpeed.y *= -1.0f;
                sound.play();
            }

            if ((ballBound.top + ballBound.height) > windowSize.y) {
                sound.setBuffer(soundDie);
                playing = false;
                sound.play();
            }
        }


        /// Handle rendering
        window.clear();

        if (playing) {
            window.draw(ball);
            window.draw(paddle);
            if (countDown > 90) {
                window.draw(readyText);
            }
            if (countDown > 0 && countDown <= 90) {
                window.draw(setText);
            }

        }
        else {
            window.draw(startText);
        }

        window.display();
    }

    return 0;
}