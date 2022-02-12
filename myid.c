#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
// #include <windows.h>

const int MAX_GROUPLIST_SIZE = 20;

void IDNameProcess (char* user_name);
void IDProcess ();

// getuid
// getgid
// getpwuid
// getgroups
// getgrgid
// getpwnam
// getgrouplist

/*
struct passwd {
	char *pw_name;    // username
	char *pw_passwd;  // user password
	uid_t pw_uid;     //user ID
	gid_t pw_gid;     // group ID
	char *pw_gecos;   // user information
	char *pw_dir;     // home directory
	char *pw_shell;   // shell program
};

struct group {
	char *gr_name;    // group name
	char *gr_passwd;  // group password
	git_t gr_gid;     // group ID
	char **gr_mem;    // NULL-terminated array of pointers to names of group members
};
*/

int main (int argc, char* argv[]) {
    if (argc == 1)
		IDProcess ();
	else
		IDNameProcess (argv[1]);
	return 0;
}

void IDProcess () {
	struct passwd* ProcessUserPsw = getpwuid(getuid());
	gid_t* process_user_groups = calloc (sizeof(gid_t), MAX_GROUPLIST_SIZE);

	int process_user_groupsnum = getgroups(MAX_GROUPLIST_SIZE, process_user_groups);

	if (process_user_groupsnum == -1)
		{
			perror ("getgroups");
			exit (EXIT_FAILURE);
		}

	printf ("uid = %d (%s) gid = %d (%s) groups = %d (%s)", ProcessUserPsw -> pw_uid, ProcessUserPsw -> pw_name, getgid(), 
		    getgrgid (getgid()) -> gr_name, ProcessUserPsw -> pw_gid, getgrgid (ProcessUserPsw -> pw_gid) -> gr_name);

	for (size_t i = 0; i < process_user_groupsnum - 1; i++)
	{
		printf (", %d (%s)", process_user_groups[i], getgrgid (process_user_groups[i]) -> gr_name);
	}

	printf ("\n");
}


void IDNameProcess (char* user_name) {
	struct passwd* UserPsw = getpwnam(user_name);

	if (!UserPsw)
	{
		perror ("getpwnam");
		exit (EXIT_FAILURE);
	}

	gid_t* user_groups = calloc (sizeof(gid_t), MAX_GROUPLIST_SIZE);

	int user_groupsnumber = MAX_GROUPLIST_SIZE;

	int user_groupsnum = getgrouplist (user_name, UserPsw->pw_gid, user_groups, &user_groupsnumber);

	if (user_groupsnum == -1)
		{
			perror ("getgrouplist");
			exit (EXIT_FAILURE);
		}

	printf ("uid= %d (%s) gid= %d (%s) groups = %d (%s)", UserPsw -> pw_uid, UserPsw -> pw_name, UserPsw -> pw_gid, 
		    getgrgid (UserPsw -> pw_gid) -> gr_name, UserPsw -> pw_gid, getgrgid (UserPsw -> pw_gid) -> gr_name);

	for (int i = 1; i < user_groupsnum; i++)
	{
		printf (", %d (%s)", user_groups[i], getgrgid (user_groups[i]) -> gr_name);
	}

	printf ("\n");
}