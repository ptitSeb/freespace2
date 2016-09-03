#include "pstypes.h"
#include "osregistry.h"
#include "cfile.h"
#undef malloc
#undef free
#undef strdup

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

const char *Osreg_company_name = "Volition";
#if defined(MAKE_FS1)
const char *Osreg_class_name = "FreespaceClass";
#else
const char *Osreg_class_name = "Freespace2Class";
#endif
#if defined(FS1_DEMO)
const char *Osreg_app_name = "FreeSpaceDemo";
const char *Osreg_title = "Freespace Demo";
#ifndef __APPLE__
const char *Osreg_user_dir = ".freespace_demo";
#else
const char *Osreg_user_dir = "Library/Application Support/FreeSpace Demo";
#endif
#define PROFILE_NAME "FreeSpaceDemo.ini"
#elif defined(FS2_DEMO)
const char *Osreg_app_name = "FreeSpace2Demo";
const char *Osreg_title = "Freespace 2 Demo";
#ifndef __APPLE__
const char *Osreg_user_dir = ".freespace2_demo";
#else
const char *Osreg_user_dir = "Library/Application Support/Freespace 2 Demo";
#endif
#define PROFILE_NAME "FreeSpace2Demo.ini"
#elif defined(OEM_BUILD)
const char *Osreg_app_name = "FreeSpace2OEM";
const char *Osreg_title = "Freespace 2 OEM";
#define PROFILE_NAME "FreeSpace2OEM.ini"
#elif defined(MAKE_FS1)
const char *Osreg_app_name = "FreeSpace";
const char *Osreg_title = "FreeSpace";
#ifndef __APPLE__
const char *Osreg_user_dir = ".freespace";
#else
const char *Osreg_user_dir = "Library/Application Support/FreeSpace";
#endif
#define PROFILE_NAME "FreeSpace.ini"
#else
const char *Osreg_app_name = "FreeSpace2";
const char *Osreg_title = "Freespace 2";
#ifndef __APPLE__
const char *Osreg_user_dir = ".freespace2";
#else
const char *Osreg_user_dir = "Library/Application Support/Freespace 2";
#endif
#define PROFILE_NAME "FreeSpace2.ini"
#endif

#define DEFAULT_SECTION "Default"


typedef struct KeyValue
{
	char *key;
	char *value;
	
	struct KeyValue *next;
} KeyValue;

typedef struct Section
{
	char *name;
	
	struct KeyValue *pairs;
	struct Section *next;
} Section;
	
typedef struct Profile
{
	struct Section *sections;
} Profile;

static char *read_line_from_file(CFILE *fp)
{
	char *buf, *buf_start;
	int buflen, len, eol;
	
	buflen = 80;
	buf = (char *)malloc(buflen);
	buf_start = buf;
	eol = 0;
	
	do {
		if (buf == NULL) {
			return NULL;
		}
		
		if (cfgets(buf_start, 80, fp) == NULL) {
			if (buf_start == buf) {
				free(buf);
				return NULL;
			} else {
				*buf_start = 0;
				return buf;
			}
		}
		
		len = strlen(buf_start);
		
		if (buf_start[len-1] == '\n') {
			buf_start[len-1] = 0;
			eol = 1;
		} else {
			buflen += 80;
			
			buf = (char *)realloc(buf, buflen);
			
			/* be sure to skip over the proper amount of nulls */
			buf_start = buf+(buflen-80)-(buflen/80)+1;
		}
	} while (!eol);
	
	return buf;
}

static char *trim_string(char *str)
{
	char *ptr;
	int len;
	
	if (str == NULL)
		return NULL;
	
	/* kill any comment */
	ptr = strchr(str, ';');
	if (ptr)
		*ptr = 0;
	ptr = strchr(str, '#');
	if (ptr)
		*ptr = 0;
	
	ptr = str;
	len = strlen(str);
	if (len > 0) {
		ptr += len-1;
	}
	
	while ((ptr > str) && isspace(*ptr)) {
		ptr--;
	}
	
	if (*ptr) {
		ptr++;
		*ptr = 0;
	}
	
	ptr = str;
	while (*ptr && isspace(*ptr)) {
		ptr++;
	}
	
	return ptr;
}

static Profile *profile_read(const char *file)
{
	CFILE *fp = cfopen(file, "rt", CFILE_NORMAL, CF_TYPE_ROOT);
	if (fp == NULL)
		return NULL;
	
	Profile *profile = (Profile *)malloc(sizeof(Profile));
	profile->sections = NULL;
	
	Section **sp_ptr = &(profile->sections);
	Section *sp = NULL;

	KeyValue **kvp_ptr = NULL;
		
	char *str;
	while ((str = read_line_from_file(fp)) != NULL) {
		char *ptr = trim_string(str);
		
		if (*ptr == '[') {
			ptr++;
			
			char *pend = strchr(ptr, ']');
			if (pend != NULL) {
				// if (pend[1]) { /* trailing garbage! */ }
				
				*pend = 0;				
				
				if (*ptr) {
					sp = (Section *)malloc(sizeof(Section));
					sp->next = NULL;
				
					sp->name = strdup(ptr);
					sp->pairs = NULL;
					
					*sp_ptr = sp;
					sp_ptr = &(sp->next);
					
					kvp_ptr = &(sp->pairs);
				} // else { /* null name! */ }
			} // else { /* incomplete section name! */ }
		} else {
			if (*ptr) {
				char *key = ptr;
				char *value = NULL;
				
				ptr = strchr(ptr, '=');
				if (ptr != NULL) {
					*ptr = 0;
					ptr++;
					
					value = ptr;
				} // else { /* random garbage! */ }
				
				if (key && *key && value /* && *value */) {
					if (sp != NULL) {
						KeyValue *kvp = (KeyValue *)malloc(sizeof(KeyValue));
						
						kvp->key = strdup(key);
						kvp->value = strdup(value);
						
						kvp->next = NULL;
						
						*kvp_ptr = kvp;
						kvp_ptr = &(kvp->next);
					} // else { /* key/value with no section! */
				} // else { /* malformed key/value entry! */ }
			} // else it's just a comment or empty string
		}
				
		free(str);
	}
	
	cfclose(fp);

	return profile;
}

static void profile_free(Profile *profile)
{
	if (profile == NULL)
		return;
		
	Section *sp = profile->sections;
	while (sp != NULL) {
		Section *st = sp;
		KeyValue *kvp = sp->pairs;
		
		while (kvp != NULL) {
			KeyValue *kvt = kvp;
			
			free(kvp->key);
			free(kvp->value);
			
			kvp = kvp->next;
			free(kvt);
		}
		
		free(sp->name);
		
		sp = sp->next;
		free(st);
	}
	
	free(profile);
}

static Profile *profile_update(Profile *profile, const char *section, const char *key, const char *value)
{
	if (profile == NULL) {
		profile = (Profile *)malloc(sizeof(Profile));
		
		profile->sections = NULL;
	}
	
	KeyValue *kvp;
	
	Section **sp_ptr = &(profile->sections);
	Section *sp = profile->sections;
	while (sp != NULL) {
		if (strcmp(section, sp->name) == 0) {
			KeyValue **kvp_ptr = &(sp->pairs);
			kvp = sp->pairs;
			
			while (kvp != NULL) {
				if (strcmp(key, kvp->key) == 0) {
					free(kvp->value);
					
					if (value == NULL) {
						*kvp_ptr = kvp->next;
						
						free(kvp->key);
						free(kvp);
					} else {
						kvp->value = strdup(value);
					}
					
					/* all done */
					return profile;
				}
				
				kvp_ptr = &(kvp->next);
				kvp = kvp->next;
			}
			
			if (value != NULL) {
				/* key not found */
				kvp = (KeyValue *)malloc(sizeof(KeyValue));
				kvp->next = NULL;
				kvp->key = strdup(key);
				kvp->value = strdup(value);
			}
					
			*kvp_ptr = kvp;
			
			/* all done */
			return profile;
		}
		
		sp_ptr = &(sp->next);
		sp = sp->next;
	}
	
	/* section not found */
	sp = (Section *)malloc(sizeof(Section));
	sp->next = NULL;
	sp->name = strdup(section);
	
	kvp = (KeyValue *)malloc(sizeof(KeyValue));
	kvp->next = NULL;
	kvp->key = strdup(key);
	kvp->value = strdup(value);
	
	sp->pairs = kvp;
	
	*sp_ptr = sp;
	
	return profile;
}

static const char *profile_get_value(Profile *profile, const char *section, const char *key)
{
	if (profile == NULL)
		return NULL;
	
	Section *sp = profile->sections;
	while (sp != NULL) {
		if (strcmp(section, sp->name) == 0) {
			KeyValue *kvp = sp->pairs;
		
			while (kvp != NULL) {
				if (strcmp(key, kvp->key) == 0) {
					return kvp->value;
				}
				kvp = kvp->next;
			}
		}
		
		sp = sp->next;
	}
	
	/* not found */
	return NULL;
}

static void profile_save(Profile *profile, const char *file)
{
	CFILE *fp;
	
	char tmp[MAX_PATH] = "";
	char tmp2[MAX_PATH] = "";
	
	if (profile == NULL)
		return;
		
	fp = cfopen(file, "wt", CFILE_NORMAL, CF_TYPE_ROOT);
	if (fp == NULL)
		return;
	
	Section *sp = profile->sections;
	while (sp != NULL) {
		sprintf(tmp, NOX("[%s]\n"), sp->name);
		cfputs(tmp, fp);
		
		KeyValue *kvp = sp->pairs;
		while (kvp != NULL) {
			sprintf(tmp2, NOX("%s=%s\n"), kvp->key, kvp->value);
			cfputs(tmp2, fp);
			kvp = kvp->next;
		}
		
		cfwrite_char('\n', fp);
		
		sp = sp->next;
	}
	
	cfclose(fp);
}

static char tmp_string_data[1024];

const char *os_config_read_string(const char *section, const char *name, const char *default_value)
{
	Profile *p = profile_read(PROFILE_NAME);

	if (section == NULL)
		section = DEFAULT_SECTION;
		
	const char *ptr = profile_get_value(p, section, name);
	if (ptr != NULL) {
		strncpy(tmp_string_data, ptr, 1023);
		default_value = tmp_string_data;
	}
	
	profile_free(p);
	
	return default_value;
}

unsigned int os_config_read_uint(const char *section, const char *name, unsigned int default_value)
{
	Profile *p = profile_read(PROFILE_NAME);
	
	if (section == NULL)
		section = DEFAULT_SECTION;
		
	const char *ptr = profile_get_value(p, section, name);
	if (ptr != NULL) {
		default_value = atoi(ptr);
	}
	
	profile_free(p);
	
	return default_value;
}

void os_config_write_string(const char *section, const char *name, const char *value)
{
	Profile *p = profile_read(PROFILE_NAME);
	
	if (section == NULL)
		section = DEFAULT_SECTION;
		
	p = profile_update(p, section, name, value);
	profile_save(p, PROFILE_NAME);
	profile_free(p);	
}

void os_config_write_uint(const char *section, const char *name, unsigned int value)
{
	static char buf[21];
	
	snprintf(buf, 20, "%u", value);
	
	Profile *p = profile_read(PROFILE_NAME);

	if (section == NULL)
		section = DEFAULT_SECTION;
	
	p = profile_update(p, section, name, buf);
	profile_save(p, PROFILE_NAME);
	profile_free(p);
}
