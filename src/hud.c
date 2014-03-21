#include "HUD.h"

#include "controller.h"
#include "textures.h"
#include "item.h"
#include "game.h"

void HUD_draw(icmmGame* game, float dt)
{
	item_draw(game->player->inv->item, dt);
}