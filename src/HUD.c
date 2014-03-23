#include "HUD.h"

#include "controller.h"
#include "textures.h"
#include "item.h"
#include "game.h"

void HUD_draw(struct icmmGame* game, float dt)
{
	
	item_draw(game->player->inv->item, dt);
	//glColor3f(1.0f, 0.0f, 0.0f, 1.0f);
}
