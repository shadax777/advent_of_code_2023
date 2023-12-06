// https://adventofcode.com/2023/day/5

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>


#define MAX_SEEDS		32
#define MAX_MAPS		16
#define MAX_MAP_ENTRIES	256


// list of entries in a single map
typedef struct mapEntry_s
{
	unsigned int dst;
	unsigned int src;
	unsigned int len;
	struct mapEntry_s *next;
} mapEntry_t;


typedef struct
{
	mapEntry_t *head;
} map_t;


static unsigned int	g_seeds[MAX_SEEDS];
static int			g_numSeeds;

// treated as list of maps in order of appearance in the data file
static map_t		g_maps[MAX_MAPS];
static int			g_numMaps;

// treated as a pool as maps build up their list of entries
static mapEntry_t	g_mapEntries[MAX_MAP_ENTRIES];
static int			g_numMapEntries;


// returns the remaining length (NOT the number of trimmed characters)
static size_t RTrim(char *str)
{
	size_t len = strlen(str);
	char *c = (str + len) - 1;

	while(len > 0 && isspace(*c))
	{
		*c-- = '\0';
		len--;
	}

	return len;
}

static const char *SkipIfStartsWith(const char *str, const char *prefixToCheck)
{
	return strstr(str, prefixToCheck) ? str + strlen(prefixToCheck) : NULL;
}

static const char *ParseNextUInt(const char *str, unsigned int *out)
{
	// skip leading spaces
	while(isspace(*str))
		str++;

	if(!*str)
		return NULL;

	*out = 0;

	while(isdigit(*str))
		*out = *out * 10 + *str++ - '0';

	return str;
}

static void LoadFile()
{
	FILE *f = fopen("5a_input.txt", "r");

	while(1)
	{
		char line[256];
		const char *s;
		int n;
		mapEntry_t *entry;

		fgets(line, 256, f);
		if(feof(f))
			break;

		if(RTrim(line) == 0)
			continue;

		// parse the seeds?
		if((s = SkipIfStartsWith(line, "seeds:")))
		{
			unsigned int seed;

			for(s = ParseNextUInt(s, &seed); s; s = ParseNextUInt(s, &seed))
			{
				assert(g_numSeeds < MAX_SEEDS);
				g_seeds[g_numSeeds++] = seed;
			}
			continue;
		}

		// create a new map?
		if(strstr(line, " map:"))
		{
			assert(g_numMaps < MAX_MAPS);

			g_numMaps++;
			continue;
		}

		// create a new map entry
		assert(g_numMapEntries < MAX_MAP_ENTRIES);
		entry = g_mapEntries + g_numMapEntries++;

		// parse the ranges
		n = sscanf(line, "%u %u %u", &entry->dst, &entry->src, &entry->len);
		assert(n == 3);

		// add the new entry to the map
		assert(g_numMaps > 0);
		entry->next = g_maps[g_numMaps - 1].head;
		g_maps[g_numMaps - 1].head = entry;
	}
	fclose(f);
}

static unsigned int FindDestinationBySourceInMapEntries(unsigned int source, const mapEntry_t *head)
{
	const mapEntry_t *e;

	for(e = head; e; e = e->next)
	{
		if(source >= e->src && source < e->src + e->len)
		{
			unsigned int offset = source - e->src;
			return e->dst + offset;
		}
	}

	// no special handling since no mapping
	return source;
}

// - the seed is always stored in the first map
// - the location is always stored in the last map
static unsigned int GetLocationBySeed(unsigned int seed)
{
	unsigned int source = seed;
	int i;

	for(i = 0; i < g_numMaps; i++)
	{
		source = FindDestinationBySourceInMapEntries(source, g_maps[i].head);
	}

	// source is now what the next destination would be if there were more maps

	return source;
}

static unsigned Min(unsigned int a, unsigned b)
{
	return a < b ? a : b;
}

int main()
{
	unsigned int shortestLocation = -1;
	int i;

	LoadFile();

	for(i = 0; i < g_numSeeds; i++)
	{
		unsigned int location = GetLocationBySeed(g_seeds[i]);
		shortestLocation = Min(shortestLocation, location);
	}

	printf("shortestLocation: %u\n", shortestLocation);

	return 0;
}
