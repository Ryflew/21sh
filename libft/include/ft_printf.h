/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/25 22:55:15 by vdarmaya          #+#    #+#             */
/*   Updated: 2016/12/04 20:19:51 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <wchar.h>

typedef struct		s_printf
{
	char			flag_sharp;
	char			flag_zero;
	char			flag_minus;
	char			flag_plus;
	char			flag_space;
	int				width;
	int				precision;
	char			*length;
	char			conversion;
}					t_printf;

int					ft_printf(const char *restrict format, ...);
int					treat(t_printf *elem, va_list ap);
int					ft_c(int c, t_printf *elem);
int					ft_s(char *str, t_printf *elem);
int					ft_smaj(wchar_t *str, t_printf *elem);
int					ft_d(int nbr, t_printf *elem);
int					ft_l(long nbr, t_printf *elem);
int					ft_x(unsigned long nbr, char letter, t_printf *elem);
int					ft_o(unsigned long nbr, t_printf *elem);
int					ft_p(void *address, t_printf *elem);
int					check_conv(char **str, char *cha, t_printf *elem);
int					ft_percent(t_printf *elem);
int					is_null(t_printf *elem);
int					ft_cwl(wint_t c, t_printf *elem);
int					ft_count_unicode(wchar_t c);
int					ft_count_multiple_unicode(wchar_t *str);
int					ft_oux(unsigned long nbr, char letter, t_printf *elem);
int					ft_invalid_input(char c, t_printf *elem);
int					end_o(char *str, int count, t_printf *elem);
char				*check_flags(char **str, t_printf *elem);
char				*check_width(char **str, t_printf *elem);
char				*check_precision(char **str, t_printf *elem);
char				*check_len(char **s, t_printf *elem);
void				ft_putnbrwp(long nbr, int precision);
void				ft_putunbrwp(unsigned long nbr, int precision);
void				ft_putstrwp(char *str, int precision);
void				two_bytes(wchar_t c);
void				three_bytes(wchar_t c);
void				four_bytes(wchar_t c);
void				ft_unicode(unsigned int c);

#endif
