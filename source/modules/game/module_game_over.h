#pragma once

#include "modules/module.h"
#include "entity/entity.h"



class CModuleGameOver : public IModule
{

public:

	CModuleGameOver(const std::string& aname) : IModule(aname) { }
	bool start() override;
	void stop() override;
	void update(float delta) override;
	void renderDebug() override;
	void renderInMenu() override;

private:
	
};

