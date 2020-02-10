#pragma once

// mobs 101 - 199; craft objects 201 - 299; world objects 301 - 399; flowers 401 - 499; auxiliary objects 501 - 599;
enum class Tag {
	hero1 = 101, hare = 102, owl = 103, deer = 104, fox = 105, bear = 106, wolf = 107, monster = 108, owlBoss = 109, nightmare1 = 110, nightmare2 = 111, nightmare3 = 112,
	heroBag = 201, noose = 202, totem = 211, hareTrap = 216, fence = 218, inkyBlackPen = 219, 
	unknownWreath = 251, hareWreath = 252, owlWreath = 253, tree = 301, grass = 302, spawn = 303,
	ground = 311, groundConnection = 312, brazier = 314, wreathTable = 315, rock = 317, 
	stump = 319, droppedLoot = 320, mushroom = 321, log = 322, bush = 323, roof = 324, lake = 325, root = 326,
	chamomile = 401, yarrow = 402, fern = 403, mugwort = 404, poppy = 405, buildObject = 501, dropPoint = 502, emptyDraft = 503, emptyPage = 504, emptyCell = 505, selectedCell = 506, clapWhirl = 507, emptyObject = 508
};

enum class PackTag
{
    empty = 0, heroAbsorb = 1, heroHit = 2, heroMove = 3, heroPick = 4, heroRoll = 5, heroStand = 6, heroThrow = 7,
    interfaceElements = 8, locations = 9, darkWoods = 10, birchGrove = 11, swampyTrees = 12, craftObjects = 13
};

enum class PackPart
{
    full = 101, body = 102, legs = 103, lines = 104, // creature's parts
    ground = 201, tree = 202, bush = 203, log = 204, mushroom = 205, path = 206, plant = 207, rock = 208, stump = 209,
    decor = 210, roof = 211, lake = 212, root = 213, flower = 213,// biome's parts
    bookLittle = 301, crossButton = 302, menu = 303, // interface's parts
    brazier = 401, wreathTable = 402, // location's parts
	cageBear = 501, cageOwl = 502, fence = 503, hareTrap = 504, noose = 505, totem = 506 // craftObject's parts
};

enum class Direction { RIGHT = 0, UPRIGHT = 45, UP = 90, UPLEFT = 135, LEFT = 180, DOWNLEFT = 225, DOWN = 270, DOWNRIGHT = 315, STAND = 360 };

enum class ButtonTag { playTag = 0, continueTag = 1, newRunTag = 2, settingsTag = 3, exitTag = 4, buildStartTag = 5, menuTag = 6, openMenu = 7,
	nextPage = 16, previousPage = 17, bookCover = 18, bookmark = 19, pageBackground = 20, pagePattern = 21, bookmarksList = 22, sketching = 46,
	bookmarkMobs = 31, bookmarkItems = 32, bookmarkWorld = 33, bookmarkFlowers = 34, bookmarkWreathes = 35, bookmarkNightmare = 36, bookButtonTag = 40,
	bookStandTag = 41, bookGlowTag = 42, bookLightningTag = 43, hpLineTag = 44, hpFrameTag = 45, cell = 47, cellSelected = 48, iconFrame1 = 49, iconFrame2 = 50, plus = 51, makeWreath = 52,
	chamomile = 401, yarrow = 402, fern = 403, mugwort = 404, poppy = 405,
	hero1 = 101, hare = 102, owl = 103, deer = 104, fox = 105, bear = 106, wolf = 107, monster = 108, owlBoss = 109,
	heroBag = 201
};

enum Actions { directHit = 2, relax = 3, combatState = 4, move = 5, dead = 6, commonHit = 9, moveHit = 10, dropping = 11, transitionToEnotherWorld = 12,
	open = 13, absorbs = 14, grab = 15, builds = 16, jerking = 17, upFlap = 30, leftFlap = 31, rightFlap = 32, startFlap = 33, stopFlap = 34, throwNoose = 35 };

enum Side { up = 1, right = 2, down = 3, left = 4 };

enum class SpeedLineDirection
{
	stand = 0,
	up_upright = 1, upright_right = 2, right_downright = 3, downright_down = 4, down_downleft = 5, downleft_left = 6, left_upleft = 7, upleft_up = 8,
	upright_up = 9, right_upright = 10, downright_right = 11, down_downright = 12, downleft_down = 13, left_downleft = 14, upleft_left = 15, up_upleft = 16
};
