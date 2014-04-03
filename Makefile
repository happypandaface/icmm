base:
	cc -c -g -o bin/stoneWall.o src/images/stoneWall.c -I./include
	cc -c -g -o bin/hand.o src/images/hand.c -I./include
	cc -c -g -o bin/gaseousBall.o src/images/gaseousBall.c -I./include
	cc -c -g -o bin/gaseousBall2.o src/images/gaseousBall2.c -I./include
	cc -c -g -o bin/blueJelly.o src/images/blueJelly.c -I./include
	cc -c -g -o bin/blueJelly2.o src/images/blueJelly2.c -I./include
	cc -c -g -o bin/deadJelly.o src/images/deadJelly.c -I./include
	cc -c -g -o bin/punchFist.o src/images/punchFist.c -I./include
	cc -c -g -o bin/levelGenerator.o src/levelGenerator.c -I./include
	cc -c -g -o bin/game.o src/game.c -I./include
	cc -c -g -o bin/util.o src/util.c -I./include
	cc -c -g -o bin/inventory.o src/inventory.c -I./include
	cc -c -g -o bin/combat.o src/combat.c -I./include
	cc -c -g -o bin/action.o src/action.c -I./include
	cc -c -g -o bin/item.o src/item.c -DFREEGLUT_STATIC -DGLEW_STATIC -I./include
	cc -c -g -o bin/textures.o src/textures.c -DFREEGLUT_STATIC -DGLEW_STATIC -I./include
	cc -c -g -o bin/controller.o src/controller.c -DFREEGLUT_STATIC -DGLEW_STATIC -I./include
	cc -c -g -o bin/creature.o src/creature.c -DFREEGLUT_STATIC -DGLEW_STATIC -I./include
	cc -c -g -o bin/tile.o src/tile.c -DFREEGLUT_STATIC -DGLEW_STATIC -I./include
	cc -c -g -o bin/HUD.o src/HUD.c -DFREEGLUT_STATIC -DGLEW_STATIC -I./include
	cc -c -g -o bin/animation.o src/animation.c -DFREEGLUT_STATIC -DGLEW_STATIC -I./include
	cc -c -g -o bin/vec_math.o src/vec_math.c -DFREEGLUT_STATIC -DGLEW_STATIC -I./include
	cc -c -g -o bin/setup.o src/setup.c -DFREEGLUT_STATIC -DGLEW_STATIC -I./include

win:
	windres my.rc -O coff -o bin/my.res
	cc -o bin/icmm.exe bin/*.o bin/my.res -L./lib -lfreeglut_static -lopengl32 -lwinmm -lgdi32 -lglu32

lin:
	cc -o bin/icmm bin/*.o -lGL -lGLU -lglut


release:
	cc -o bin/icmm.exe bin/*.o -L./lib -lfreeglut_static -lopengl32 -lwinmm -lgdi32 -lglu32 -Wl,--subsystem,windows

glew:
	gcc -c -o bin/glew.o src/other/glew.c -DGLEW_STATIC -I./include
