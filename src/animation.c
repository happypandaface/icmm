#include "animation.h"

#include <stdio.h>
#include <stdlib.h>
#include "textures.h"

void setup_animations()
{
	anm_gas_ball = malloc(sizeof(anm_gas_ball));
	anm_hand_punch = malloc(sizeof(anm_hand_punch));
	
	new_animation(anm_gas_ball);
	add_frame(anm_gas_ball, TEX_GASEOUS_BALL_1, 1.0f);
	add_frame(anm_gas_ball, TEX_GASEOUS_BALL_2, 1.0f);
	
	new_animation(anm_hand_punch);
	add_frame(anm_hand_punch, TEX_FIST, 0.1f);
	add_frame(anm_hand_punch, TEX_PUNCH, 0.4f);
}

void new_animation(Animation* anim)
{
	anim->firstFrame = NULL;
	anim->lastFrame = NULL;
}

void add_frame(Animation* anim, int texture, float frameTime)
{
	struct Frame* f = malloc(sizeof(*f));
	f->texture = texture;
	f->frameTime = frameTime;
	f->nextFrame = anim->firstFrame;
	if (anim->firstFrame == NULL)
	{
		anim->firstFrame = f;
		anim->lastFrame = f;
	}else
	{
		anim->lastFrame->nextFrame = f;
		anim->lastFrame->isLastFrame = 0;
		anim->lastFrame = f;
	}
	anim->lastFrame->isLastFrame = 1;
}

void set_instance(Animation* anim, AnimationInstance* animInst)
{
	animInst->timesLooped = 0;
	animInst->currentTime = 0;
	animInst->currentFrame = anim->firstFrame;
}

void set_animation(AnimationInstance** animInst, int anim_type)
{
	if ((*animInst) == NULL)
	{
		(*animInst) = malloc(sizeof(**animInst));
	}
	switch(anim_type)
	{
		case ANM_NONE:
			break;
		case ANM_GAS_BALL:
			set_instance(anm_gas_ball, (*animInst));
			break;
		case ANM_PUNCH:
			set_instance(anm_hand_punch, (*animInst));
			break;
	}
}

void unload_animation(AnimationInstance** animInst)
{
	free((*animInst));
	(*animInst) = NULL;
}

void step_animation(AnimationInstance* animInst, float dt)
{
	animInst->currentTime += dt;
	if (animInst->currentTime > animInst->currentFrame->frameTime)
	{
		animInst->currentTime = 0;
		if (animInst->currentFrame->isLastFrame)
			animInst->timesLooped++;
		animInst->currentFrame = animInst->currentFrame->nextFrame;
	}
}