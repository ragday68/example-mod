#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/GJGameLevel.hpp>

using namespace geode::prelude;

static bool isCheating() {
	auto mod = Mod::get();
	return mod->getSettingValue<bool>("noclip")
		|| mod->getSettingValue<double>("speed") != 1.0;
}

class $modify(MyMenuLayer, MenuLayer) {
	bool init() {
		if (!MenuLayer::init()) return false;

		auto sprite = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
		sprite->setScale(0.7f);

		auto btn = CCMenuItemSpriteExtra::create(
			sprite, this, menu_selector(MyMenuLayer::onModMenu)
		);
		btn->setID("mod-menu-button"_spr);

		if (auto menu = this->getChildByID("bottom-menu")) {
			menu->addChild(btn);
			menu->updateLayout();
		}
		return true;
	}

	void onModMenu(CCObject*) {
		openSettingsPopup(Mod::get(), false);
	}
};

class $modify(MyPlayLayer, PlayLayer) {
	void destroyPlayer(PlayerObject* player, GameObject* object) override {
		if (Mod::get()->getSettingValue<bool>("noclip") && object != m_anticheatSpike) {
			return;
		}
		PlayLayer::destroyPlayer(player, object);
	}

	void update(float dt) override {
		auto speed = static_cast<float>(Mod::get()->getSettingValue<double>("speed"));
		PlayLayer::update(dt * speed);
	}
};

class $modify(MyGameLevel, GJGameLevel) {
	void savePercentage(int percent, bool practice, int clicks, int attempts, bool valid) {
		if (isCheating()) return;
		GJGameLevel::savePercentage(percent, practice, clicks, attempts, valid);
	}
};
