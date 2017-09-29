#include "tosh.h"

static char	*fill_word(char *strf, char const *s, size_t len)
{
	size_t			i;
	char			string_op;
	size_t			new_len;
	char			bs;

	i = -1;
	new_len = 0;
	string_op = 0;
	bs = 0;
	while (++i < len)
	{
		if (!bs && is_string_op(s[i]) && !string_op)
			string_op = s[i];
		else if (s[i] == string_op && !bs)
			string_op = 0;
		else if (bs || (s[i] != string_op && s[i] != '\\'))
			strf[new_len++] = s[i];
		if (s[i] == '\\' && string_op != '\'' && !bs)
			bs = 1;
		else
			bs = 0;
	}
	strf[new_len] = '\0';
	return (strf);
}

char	*get_word(char const *s, size_t len)
{
	char			*strf;
	size_t			i;
	char			string_op;
	size_t			new_len;
	char			bs;

	if (!s)
		return (NULL);
	i = -1;
	new_len = 0;
	string_op = 0;
	bs = 0;
	while (++i < len)
	{
		if (!bs && is_string_op(s[i]) && !string_op)
			string_op = s[i];
		else if (s[i] == string_op && !bs)
			string_op = 0;
		else if (bs || (s[i] != string_op && s[i] != '\\'))
			++new_len;
		bs = (s[i] == '\\' && string_op != '\'' && !bs) ? 1 : 0;
	}
	if ((strf = (char*)malloc(sizeof(char) * (new_len + 1))))
		return (fill_word(strf, s, len));
	return (NULL);		
}