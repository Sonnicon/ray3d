#ifndef H_WORLD
#define H_WORLD

// Normalize an edge from 0-3
#define NORMALIZE_FACE(e) ((e)%4 + ((e)<0) * 4)
// Distance to edge f from position p in block b
#define DISTANCE_TO_WALL(f, p, b) (((f)<=1)*(p)[(f)%2]+((f)>=2)*((b)->widths[(f)%2]-(p)[(f)%2]))
// Normalize an angle to 0-2pi
#define NORMALIZE_ANGLE(a) (fmodf((a), 2.f * M_PI) + ((a)<0.) * 2.f * M_PI)

struct World_Block {
	struct World_Block *nearby_blocks[4];
	unsigned char nearby_blocks_faces[4];
	unsigned char face_textures[4];
	unsigned char widths[2];
};

struct World_Position {
	double x;
	double y;
	double angle;
	struct World_Block *block;
};

struct World_Face {
	struct World_Block *block;
	unsigned char face;
};

struct World_Block *block_copy(struct World_Block *from);

#define uc unsigned char
#define bp(x) struct World_Block *nearby_block##x

struct World_Block *
block_create(uc w, uc h, bp(0), bp(1), bp(2), bp(3), char face0, char face1, char face2, char face3);

#undef uc
#undef bp

void world_cast_distance(struct World_Position *result, struct World_Position *source, double distance);

char world_get_face(struct World_Position *source);

#endif
