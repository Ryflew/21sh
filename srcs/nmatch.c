/*int			nmatch(char *s1, char *s2)
{
	printf("s1 = %s | s2 = %s\n", s1, s2);
	if (*s2 == '*' && *s1 == '\0')
	{
		printf("1\n");
		return (nmatch(s1, s2 + 1));
	}
	if (*s2 == '*' && *s1 != '\0')
	{
		printf("2\n");
		return (nmatch(s1 + 1, s2) + nmatch(s1, s2 + 1));
	}
	if (*s1 == '\0' && *s2 == '\0' && *s2 == *s1)
	{
		printf("3\n");
		return (1);
	}
	if (*s2 == '[' && *(s2 + 1) && *(s2 + 1) == '!' && *s1 != '\0')
	{
		printf("4\n");
		while (*(++s2) && *s2 != ']')
			if (*s1 != '\0' && *s2 != '\0' && *s2 == *s1)
				return (0);
		if (*s2 != ']')
		{
			printf("bad pattern");
			return (-1);
		}
		return (nmatch(s1 + 1, s2 + 1));
	}
	if (*s2 == '[' && *s1 != '\0')
	{
		printf("4\n");
		while (*(++s2) && *s2 != ']')
			if (*s1 != '\0' && *s2 != '\0' && *s2 == *s1)
			{
				while (*(s2) && *s2 != ']')
					++s2;
				if (*s2 != ']')
				{
					printf("bad pattern");
					return (-1);
				}
				return (nmatch(s1 + 1, s2 + 1));
			}
		return (0);
	}
	if (*s2 == ']')
	{
		printf("5\n");
		return (nmatch(s1, s2 + 1));
	}
	if (*s2 == '?' && *s1 != '\0')
	{
		printf("6\n");
		return (nmatch(s1 + 1, s2 + 1));
	}
	if (*s1 != '\0' && *s2 != '\0' && *s2 == *s1 && *s2 != '?' && *s2 != '[')
	{
		printf("7\n");
		return (nmatch(s1 + 1, s2 + 1));
	}
	printf("END\n");
	return (0);
}

int main(int ac, char **av)
{
	if (ac > 2)
		printf("%d\n", nmatch(av[1], av[2]));
	return (0);
}*/

#include <stdio.h>
#include "tosh.h"

int			nmatch(char *s1, char *s2, t_list *lexems)
{
	t_token	*token;

	token = (t_token*)lexems->data;
//	printf("s1 = %s | s2 = %s\n", s1, s2);
	if (token->type == S_WILDCARD && *s1 == '\0')
	{
//		printf("1\n");
		return (nmatch(s1, NULL, lexems->next));
	}
	if (token->type == S_WILDCARD && *s1 != '\0')
	{
//		printf("2\n");
		return (nmatch(s1 + 1, NULL, lexems) + nmatch(s1, NULL, lexems->next));
	}
	if (*s1 == '\0' && (token->type == NONE || (s2 && *s2 == '/')))
	{
//		printf("3\n");
		return (1);
	}
	if (token->type == E_WILDCARD && *s1 != '\0')
	{
//		printf("4\n");
		if (lexems->next)
		{
			token = (t_token*)lexems->next->data;
			s2 = token->value;
			while (*(s2))
				if (*s1 != '\0' && *s2 != '\0' && *s2++ == *s1)
					return (0);
			return (nmatch(s1 + 1, NULL, lexems->next->next->next));					
		}
	}
	if (token->type == LBKT && *s1 != '\0')
	{
//		printf("4\n");
		if (lexems->next)
		{
			token = (t_token*)lexems->next->data;
			s2 = token->value;
			while (*(s2))
				if (*s1 != '\0' && *s2 != '\0' && *s2++ == *s1)
					return (nmatch(s1 + 1, NULL, lexems->next->next->next));
		}
	}
	if (token->type == Q_WILDCARD && *s1 != '\0')
	{
//		printf("6\n");
		return (nmatch(s1 + 1, NULL, lexems->next));
	}
	if (token->type == EXPR)
	{
		if (!s2)
			s2 = token->value;
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