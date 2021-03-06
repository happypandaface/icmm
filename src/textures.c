#include "textures.h"

void load_texture(int tex)
{
	switch (tex)
	{
		case TEX_NOTHING:
			break;
		case TEX_STONE_WALL:
			glBindTexture(GL_TEXTURE_2D, swTexture);
			break;
		case TEX_GASEOUS_BALL_1:
			glBindTexture(GL_TEXTURE_2D, gbTexture);
			break;
		case TEX_GASEOUS_BALL_2:
			glBindTexture(GL_TEXTURE_2D, gb2Texture);
			break;
		case TEX_FIST:
			glBindTexture(GL_TEXTURE_2D, handTexture);
			break;
		case TEX_PUNCH:
			glBindTexture(GL_TEXTURE_2D, punchTexture);
			break;
		case TEX_BLUE_JELLY_1:
			glBindTexture(GL_TEXTURE_2D, bJelly1Texture);
			break;
		case TEX_BLUE_JELLY_2:
			glBindTexture(GL_TEXTURE_2D, bJelly2Texture);
			break;
		case TEX_DEAD_JELLY:
			glBindTexture(GL_TEXTURE_2D, dJellyTexture);
			break;
	}
}
