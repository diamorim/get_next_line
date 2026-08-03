#include "get_next_line_.h"

size_t ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i != '\0'])
	{
		i++;
	}
	return i;
}

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	len;
	size_t	i;

	i = 0;
	len = ft_strlen(src);
	if (size > 0)
	{
		while (i < size - 1 && src[i] != '\0')
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	return (len);
}
char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	s_len;
	char	*p;

	s_len = ft_strlen(s1) + ft_strlen(s2);
	p = malloc((s_len + 1) * sizeof(char));
	if (!p)
		return (NULL);
	ft_strlcpy(p, s1, 1000);
	ft_strlcat(p, s2, s_len + 1);
	return (p);
}

char	*ft_strrchr(const char *s, int c)
{
	int	len;
	int	i;

	len = ft_strlen(s);
	i = len - 1;
	if ((char)c == 0)
		return ((char *)&s[i + 1]);
	while (i >= 0)
	{
		if (s[i] == (char)c)
			return ((char *)&s[i]);
		i--;
	}
	return (NULL);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char			*substr;
	unsigned int	i;
	unsigned int	size;

	size = ft_strlen(s);
	i = 0;
	if (start > size)
		return "";
	if (size - start >= len)
		substr = malloc((len + 1) * sizeof(char));
	else
		substr = malloc((size - start + 1) * sizeof(char));
	if (!substr)
		return (NULL);
	while (len > 0 && s[start])
	{
		substr[i] = s[start];
		i++;
		start++;
		len--;
	}
	substr[i] = '\0';
	return (substr);
}
