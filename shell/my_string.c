int my_strcmp(char *a, char *b) {
	int i = 0;

	while((a[i] != '\0') && (b[i] != '\0'))	{
		if(a[i] > b[i])	{
			return 1;
		}
		else if (a[i] < b[i]) {
			return -1;
		}
		i++;
	}

	if(a[i] == '\0') {
		if(b[i] == '\0') {
			return 0;
		}
		else {
			return -1;
		}
	}
}