#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <iostream>
struct worldInfo
{
	double k, G, g;
	double Induction;
	//to, from;
	std::string InductionDirention;
	bool collision;
};

worldInfo world;
const sf::Vector2i screenSize = sf::Vector2i(800, 800);
const double PI = 3.1415926;

const double k = 0.00005f;

struct bodyInfo
{
	int ID;
	double mass, charge;
	sf::Vector2f speed, accel;
	sf::Vector2f coord;
	sf::CircleShape sprite;
};

class body {
public:
	bodyInfo me;
	void init(bodyInfo startInfo) {
		me = startInfo;
	}

	bodyInfo update(sf::Time time, std::vector<bodyInfo> objects, sf::RenderWindow *window) {

		me.accel = sf::Vector2f(0, 0);
		generateAccel(objects);
		me.speed.x += me.accel.x * time.asMilliseconds();
		me.speed.y += me.accel.y * time.asMilliseconds();
		me.coord.x += me.speed.x * time.asMilliseconds();
		me.coord.y += me.speed.y * time.asMilliseconds();

		if (world.collision) {
			if (me.coord.x * k < 0 + me.sprite.getRadius()) {
				me.coord.x = (0 + me.sprite.getRadius()) / k;
				me.speed.x *= -0.9;
			}
			if (me.coord.x * k > screenSize.x - me.sprite.getRadius()) {
				me.coord.x = (screenSize.x - me.sprite.getRadius()) / k;
				me.speed.x *= -0.9;
			}
			if (me.coord.y * k < 0 + me.sprite.getRadius()) {
				me.coord.y = (0 + me.sprite.getRadius()) / k;
				me.speed.y *= -0.9;
			}
			if (me.coord.y * k > screenSize.y - me.sprite.getRadius()) {
				me.coord.y = (screenSize.y - me.sprite.getRadius()) / k;
				me.speed.y *= -0.9;
			}
		}
		if (me.ID == 0) std::cout << me.coord.x << " " << me.coord.y << std::endl;
		me.sprite.setPosition(me.coord.x * k, me.coord.y * k);
		window->draw(me.sprite);
		return me;
	}

private:
	void generateAccel(std::vector<bodyInfo> objects) {
		me.accel += graviPower(objects);
		me.accel += chargePower(objects);
		me.accel += magneticPower();
		me.accel += globalGraviPower();
	}

	sf::Vector2f graviPower(std::vector<bodyInfo> objects) {
		sf::Vector2f power;
		power.x = 0;
		power.y = 0;
		for (int i = 0; i < objects.size(); i++) if (objects[i].ID != me.ID) {
			double distance = sqrt(pow(me.coord.x - objects[i].coord.x, 2) + pow(me.coord.y - objects[i].coord.y, 2));

			double vPower = - world.G * objects[i].mass / pow(distance, 3);
			power.x += vPower * (me.coord.x - objects[i].coord.x);
			power.y += vPower * (me.coord.y - objects[i].coord.y);
		}
		return power;
	}

	sf::Vector2f chargePower(std::vector<bodyInfo> objects) {
		sf::Vector2f power;
		power.x = 0;
		power.y = 0;
		for (int i = 0; i < objects.size(); i++) if (objects[i].ID != me.ID) {
			double distance = sqrt(pow(me.coord.x - objects[i].coord.x, 2) + pow(me.coord.y - objects[i].coord.y, 2));

			double vPower = world.k * objects[i].charge / pow(distance, 3) * me.charge / abs(me.charge);
			power.x += vPower * (me.coord.x - objects[i].coord.x);
			power.y += vPower * (me.coord.y - objects[i].coord.y);
		}
		return power;
	}

	sf::Vector2f magneticPower() {
		sf::Vector2f power;
		if (world.InductionDirention == "to") {
			power.x = - world.Induction * me.charge * me.speed.y;
			power.y = world.Induction * me.charge * me.speed.x;
		}
		else if (world.InductionDirention == "from") {
			power.x = world.Induction * me.charge * me.speed.y;
			power.y = - world.Induction * me.charge * me.speed.x;
		}
		return power;
	}

	sf::Vector2f globalGraviPower() {
		sf::Vector2f power;
		power.x = 0;
		power.y = world.g;
		return power;
	}
};

int main()
{
	sf::RenderWindow window(sf::VideoMode(screenSize.x, screenSize.y), "SFMLworks");

	world.G = 10000;
	world.k = 10000;
	world.g = 0;
	world.Induction = 0.1;
	world.InductionDirention = "to";
	world.collision = true;

	std::vector<bodyInfo> objectsInfo(0);

	bodyInfo a1;
	a1.ID = 0;
	a1.mass = 100;
	a1.charge = 10;
	a1.coord = sf::Vector2f(400, 400);
	a1.speed = sf::Vector2f(0, 50);
	a1.accel = sf::Vector2f(0, 0);
	a1.sprite.setRadius(10);
	a1.sprite.setFillColor(sf::Color::Green);
	a1.sprite.setOrigin(sf::Vector2f(5, 5));
	objectsInfo.push_back(a1);

	a1.ID = 1;
	a1.mass = 1;
	a1.charge = -1;
	a1.coord = sf::Vector2f(300, 400);
	a1.speed = sf::Vector2f(0, 100);
	a1.accel = sf::Vector2f(0, 0);
	a1.sprite.setRadius(10);
	a1.sprite.setFillColor(sf::Color::Red);
	a1.sprite.setOrigin(sf::Vector2f(5, 5));
	//objectsInfo.push_back(a1);

	std::vector<body> objects;
	for (int i = 0; i < objectsInfo.size(); i++) {
		body a;
		a.init(objectsInfo[i]);
		objects.push_back(a);
	}
	sf::Clock clock;
	sf::Time timer;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		//window.clear();

		timer = clock.getElapsedTime();
		clock.restart();
		std::vector<bodyInfo> newInfo(0);
		for (int i = 0; i < objects.size(); i++) newInfo.push_back(objects[i].update(timer, objectsInfo, &window));
		objectsInfo = newInfo;
		newInfo.resize(0);
		window.display();
	}

	return 0;
}