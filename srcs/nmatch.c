#include <stdio.h>
#include "tosh.h"

int			nmatch(char *s1, char *s2, t_list *lexems)
{
	t_token	*token;

	token = (t_token*)lexems->data;
//	printf("s1 = %s | s2 = %s\n", s1, s2);
	if (TYPE == S_WILDCARD && *s1 == '\0')
	{
//		printf("1\n");
		return (nmatch(s1, NULL, lexems->next));
	}
	if (TYPE == S_WILDCARD && *s1 != '\0')
	{
//		printf("2\n");
		return (nmatch(s1 + 1, NULL, lexems) + nmatch(s1, NULL, lexems->next));
	}
	if (*s1 == '\0' && (TYPE == END_EXPR || (s2 && *s2 == '/')))
	{
//		printf("3\n");
		return (1);
	}
	if (TYPE == E_WILDCARD && *s1 != '\0')
	{
//		printf("4\n");
		if (lexems->next)
		{
			token = (t_token*)lexems->next->data;
			s2 = VAL;
			while (*(s2))
				if (*s1 != '\0' && *s2 != '\0' && *s2++ == *s1)
					return (0);
			return (nmatch(s1 + 1, NULL, lexems->next->next->next));
		}
	}
	if (TYPE == LBKT && *s1 != '\0')
	{
//		printf("4\n");
		if (lexems->next)
		{
			token = (t_token*)lexems->next->data;
			s2 = VAL;
			while (*(s2))
				if (*s1 != '\0' && *s2 != '\0' && *s2++ == *s1)
					return (nmatch(s1 + 1, NULL, lexems->next->next->next));
		}
	}
	if (TYPE == Q_WILDCARD && *s1 != '\0')
	{
//		printf("6\n");
		return (nmatch(s1 + 1, NULL, lexems->next));
	}
	if (TYPE == EXPR && TYPE != END_EXPR)
	{
		if (!s2)
			s2 = VAL;
		if (*s1 != '\0' && *s2 != '\0' && *s2 == *s1)
		{
//			printf("7\n");
			return (nmatch(s1 + 1, s2 + 1, lexems));
		}
		else if (!*s2)
			return (nmatch(s1, NULL, lexems->next));
		else if (*s1 == '\0' && *s2 == '/')
			return (1);
	}
//	printf("END\n");
	return (0);
}
