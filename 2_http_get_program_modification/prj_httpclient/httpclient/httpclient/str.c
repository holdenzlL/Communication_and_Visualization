#include "str.h"

str_p new_str(const char* string)
{
	str_p p = (str_p)malloc(sizeof(str));
	if (!p)
	{
		perror("Error allocating memory");
		return NULL;
	}

	p->size = sizeof_buff(string);
	p->buffer = (char*)malloc(p->size);
	if (!p->buffer)
	{
		perror("Error allocating memory");
		p->size = 0;
		free(p);
		return NULL;
	}
	for (size_t i = 0; i < p->size; i++)
	{
		p->buffer[i] = string[i];
	}
	return p;
}

void free_str(str_p p)
{
	if (p->buffer)
		free(p->buffer);
	if (p)
		free(p);
	p->size = 0;
	p->buffer = NULL;
	p = NULL;
	return;
}

str_p cpy_str(const str_p p)
{
	return new_str(p->buffer);
}

size_t size_str(const str_p p)
{
	return p->size;
}

str_p zero_str(size_t size)
{
	str_p p = (str_p)malloc(sizeof(str));
	if (!p)
	{
		perror("Error allocating memory");
		return NULL;
	}

	p->size = size;
	p->buffer = (char*)calloc(size, sizeof(char));
	if (!p->buffer)
	{
		perror("Error allocating memory");
		p->size = 0;
		free(p);
		return NULL;
	}
	return p;
}

str_p revs_str(const str_p p)
{
	str_p q = zero_str(p->size);
	if (!q)
		return NULL;
	if (p->size)
	{
		for (size_t i = 0; i < p->size - 1; i++)
		{
			q->buffer[i] = p->buffer[p->size - 2 - i];
		}
		q->buffer[p->size - 1] = '\0';
	}
	return q;
}

size_t sizeof_buff(const char* string)
{
	//cant process NULL
	size_t i = 1;
	while (*string != '\0')
	{
		i++;
		string++;
	}
	return i;
}

str_p overwrite_str(str_p p, const char* string)
{
	free_str(p);
	return new_str(string);
}

void swp_str(str_p p, str_p q)
{
	const str_p tmp = p;
	*p = *q;
	*q = *tmp;
	return;
}

int cmp_str(const str_p p, const str_p q)
{
	const int n = memcmp_str(p->buffer, q->buffer, (p->size < q->size) ? p->size : q->size);

	if (n != 0 || p->size == q->size)
		return n;

	return (p->size < q->size) ? -1 : 1;
}

str_p readin_file_str(FILE* fp)
{
	rewind(fp);
	size_t size = 1;  //additional '\0' for appending to the end of the string

	fseek(fp, 0, SEEK_END);
	size += ftell(fp);
	rewind(fp);
	str_p p = zero_str((size_t)size);

	if (size - 1 != fread(p->buffer, sizeof(char), size, fp))	//should be size -1 also
	{
		perror("Error reading file");
		free_str(p);
		return NULL;
	}

	p->buffer[size - 1] = '\0';
	return p;
}

str_p append_str(str_p p, const char* string)
{
	size_t i = 0;
	size_t size = sizeof_buff(string);
	str_p new_p = (str_p)malloc(sizeof(str));
	if (!new_p)
	{
		perror("Error allocating memory");
		return NULL;
	}

	new_p->size = size + p->size - 1;
	new_p->buffer = (char*)malloc(new_p->size);
	if (!p->buffer)
	{
		perror("Error allocating memory");
		p->size = 0;
		free(p);
		return NULL;
	}
	for (i = 0; i < p->size - 1; i++)
	{
		new_p->buffer[i] = p->buffer[i];
	}
	for (i = 0; i < size; i++)
	{
		new_p->buffer[p->size - 1 + i] = string[i];
	}
	return new_p;
}

int memcmp_str(const char* s1, const char* s2, size_t n)
{
	if (n != 0) {
		register const unsigned char* p1 = s1, * p2 = s2;
		do {
			if (*p1++ != *p2++)
				return (*--p1 - *--p2);
		} while (--n != 0);
	}
	return (0);
}

str_p swap_chr(str_p p, unsigned long index1, unsigned long index2)
{
	if (index1 > p->size - 1 || index2 > p->size - 1)
	{
		perror("Error input");
		return p;
	}
	char tmp = p->buffer[index1];
	p->buffer[index1] = p->buffer[index2];
	p->buffer[index2] = tmp;
	return p;
}

size_t partition(char* arr, size_t low, size_t high)
{
	char pivot = arr[low];

	for (;;) {
		while (low < high && pivot <= arr[high]) {
			high--;
		}
		if (low >= high) break;
		arr[low++] = arr[high];

		while (low < high && arr[low] <= pivot) {
			low++;
		}
		if (low >= high) break;
		arr[high--] = arr[low];
	}

	arr[high] = pivot;
	return high;
}

void quickSort(char* arr, size_t low, size_t high)
{
	size_t pi;

	if (low < high)
	{
		pi = partition(arr, low, high);
		if (pi)
		{
			quickSort(arr, low, (pi - 1));
		}
		quickSort(arr, (pi + 1), high);
	}
}

str_p nsort_str(str_p p, size_t low, size_t high)
{
	if (low < 0 || high > p->size)
	{
		perror("Invalid index");
		return p;
	}
	quickSort(p->buffer, low, high);
	return p;
}

str_p sort_str(str_p p)
{
	quickSort(p->buffer, 0, p->size - 2);
	return p;
}

//void write2_file_str(const char* filename, const str_p p)
//{
//	FILE* fp;
//	errno_t err = fopen_s(&fp, filename, "ab+");
//	if (err)
//	{
//		perror("Error opening file");
//		return;
//	}
//	if (p->size - 1 != fwrite(p->buffer, sizeof(char), p->size - 1, fp))
//	{
//		perror("Error writing file");
//		fclose(fp);
//		return;
//	}
//	fclose(fp);
//	return p;
//}

int write2_file_str(FILE*fp, const str_p p)
{
	if (p->size - 1 != fwrite(p->buffer, sizeof(char), p->size - 1, fp))
	{
		perror("Error writing file");
		return 1;
	}
	return 0;
}

// search string for substring
//If string contains substring, the return value is the
//location of the first matching instance of substring
//in string.If string doesn't contain substring, the
//return value is 0.  Matching is done on an exact
//character - for - character basis with no wildcards or special
//characters.
char* srh_str(const str_p string_origin, const char* substring)
{
	register char* string = string_origin->buffer;
	register char* a, * b;

	b = substring;
	if (*b == 0) {
		return string;
	}
	for (; *string != 0; string += 1) {
		if (*string != *b) {
			continue;
		}
		a = string;
		while (1) {
			if (*b == 0) {
				return string;
			}
			if (*a++ != *b++) {
				break;
			}
		}
		b = substring;
	}
	return NULL;
}

int cmp_charArray_(const char* p1, const char* p2)
{
	const char* s1 = p1;
	const char* s2 = p2;
	char c1, c2;

	do
	{
		c1 = (unsigned char)*s1++;
		c2 = (unsigned char)*s2++;
		if (c1 == '\0')
			return c1 - c2;
	} while (c1 == c2);

	return c1 - c2;
}

size_t chk_str(str_p* p)
{
	register const char* s;

	for (s = (*p)->buffer; *s; ++s);	//s is the position of '\0'

	if (s - (*p)->buffer != (*p)->size)
	{
		str_p tmp = new_str((*p)->buffer);
		free_str(*p);
		*p = tmp;
	}
	return (*p)->size;

}
