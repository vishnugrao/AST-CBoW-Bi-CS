}
static GList *msn_logger_list(PurpleLogType type, const char *sn, PurpleAccount *account)
{
	GList *list = NULL;
	char *username;
	PurpleBuddy *buddy;
	const char *logdir;
	const char *savedfilename = NULL;
	char *logfile;
	char *path;
	GError *error = NULL;
	gchar *contents = NULL;
	gsize length;
	xmlnode *root;
	xmlnode *message;
	const char *old_session_id = "";
	struct msn_logger_data *data = NULL;
	g_return_val_if_fail(sn != NULL, NULL);
	g_return_val_if_fail(account != NULL, NULL);
	if (strcmp(account->protocol_id, "prpl-msn"))
		return NULL;
	logdir = purple_prefs_get_string("/plugins/core/log_reader/msn/log_directory");
	/* By clearing the log directory path, this logger can be (effectively) disabled. */
	if (!logdir || !*logdir)
		return NULL;
	buddy = purple_find_buddy(account, sn);
	if ((username = g_strdup(purple_account_get_string(
			account, "log_reader_msn_log_folder", NULL)))) {
		/* As a special case, we allow the null string to kill the parsing
		 * straight away. This would allow the user to deal with the case
		 * when two account have the same username at different domains and
		 * only one has logs stored.
		 */
		if (!*username) {
			g_free(username);
			return list;
		}
	} else {
		username = g_strdup(purple_normalize(account, account->username));
	}
	if (buddy) {
		savedfilename = purple_blist_node_get_string((PurpleBlistNode *)buddy,
		                                             "log_reader_msn_log_filename");
	}
	if (savedfilename) {
		/* As a special case, we allow the null string to kill the parsing
		 * straight away. This would allow the user to deal with the case
		 * when two buddies have the same username at different domains and
		 * only one has logs stored.
		 */
		if (!*savedfilename) {
			g_free(username);
			return list;
		}
		logfile = g_strdup(savedfilename);
	} else {
		logfile = g_strdup_printf("%s.xml", purple_normalize(account, sn));
	}
	path = g_build_filename(logdir, username, "History", logfile, NULL);
	if (!g_file_test(path, G_FILE_TEST_EXISTS)) {
		gboolean found = FALSE;
		char *at_sign;
		GDir *dir;
		g_free(path);
		if (savedfilename) {
			/* We had a saved filename, but it doesn't exist.
			 * Returning now is the right course of action because we don't
			 * want to detect another file incorrectly.
			 */
			g_free(username);
			g_free(logfile);
			return list;
		}
		/* Perhaps we're using a new version of MSN with the weird numbered folders.
		 * I don't know how the numbers are calculated, so I'm going to attempt to
		 * find logs by pattern matching...
		 */
		at_sign = g_strrstr(username, "@");
		if (at_sign)
			*at_sign = '\0';
		dir = g_dir_open(logdir, 0, NULL);
		if (dir) {
			const gchar *name;
			while ((name = g_dir_read_name(dir))) {
				const char *c = name;
				if (!purple_str_has_prefix(c, username))
					continue;
				c += strlen(username);
				while (*c) {
					if (!g_ascii_isdigit(*c))
						break;
					c++;
				}
				path = g_build_filename(logdir, name, NULL);
				/* The !c makes sure we got to the end of the while loop above. */
				if (!*c && g_file_test(path, G_FILE_TEST_IS_DIR)) {
					char *history_path = g_build_filename(
						path,  "History", NULL);
					if (g_file_test(history_path, G_FILE_TEST_IS_DIR)) {
						purple_account_set_string(account,
							"log_reader_msn_log_folder", name);
						g_free(path);
						path = history_path;
						found = TRUE;
						break;
					}
					g_free(path);
					g_free(history_path);
				}
				else
					g_free(path);
			}
			g_dir_close(dir);
		}
		g_free(username);
		if (!found) {
			g_free(logfile);
			return list;
		}
		/* If we've reached this point, we've found a History folder. */
		username = g_strdup(purple_normalize(account, sn));
		at_sign = g_strrstr(username, "@");
		if (at_sign)
			*at_sign = '\0';
		found = FALSE;
		dir = g_dir_open(path, 0, NULL);
		if (dir) {
			const gchar *name;
			while ((name = g_dir_read_name(dir))) {
				const char *c = name;
				if (!purple_str_has_prefix(c, username))
					continue;
				c += strlen(username);
				while (*c) {
					if (!g_ascii_isdigit(*c))
						break;
					c++;
				}
				path = g_build_filename(path, name, NULL);
				if (!strcmp(c, ".xml") &&
				    g_file_test(path, G_FILE_TEST_EXISTS)) {
					found = TRUE;
					g_free(logfile);
					logfile = g_strdup(name);
					break;
				}
				else
					g_free(path);
			}
			g_dir_close(dir);
		}
		g_free(username);
		if (!found) {
			g_free(logfile);
			return list;
		}
	} else {
		g_free(username);
		g_free(logfile);
		logfile = NULL; /* No sense saving the obvious buddy@domain.com. */
	}
	purple_debug_info("MSN log read", "Reading %s\n", path);
	if (!g_file_get_contents(path, &contents, &length, &error)) {
		g_free(path);
		purple_debug_error("MSN log read", "Error reading log\n");
		if (error)
			g_error_free(error);
		return list;
	}
	g_free(path);
	/* Reading the file was successful...
	 * Save its name if it involves the crazy numbers. The idea here is that you could
	 * then tweak the blist.xml file by hand if need be. This would be the case if two
	 * buddies have the same username at different domains. One set of logs would get
	 * detected for both buddies.
	 */
	if (buddy && logfile) {
		PurpleBlistNode *node = (PurpleBlistNode *)buddy;
		purple_blist_node_set_string(node, "log_reader_msn_log_filename", logfile);
		g_free(logfile);
	}
	root = xmlnode_from_str(contents, length);
	g_free(contents);
	if (!root)
		return list;
	for (message = xmlnode_get_child(root, "Message"); message;
			message = xmlnode_get_next_twin(message)) {
		const char *session_id;
		session_id = xmlnode_get_attrib(message, "SessionID");
		if (!session_id) {
			purple_debug_error("MSN log parse",
			                   "Error parsing message: %s\n", "SessionID missing");
			continue;
		}
		if (strcmp(session_id, old_session_id)) {
			/*
			 * The session ID differs from the last message.
			 * Thus, this is the start of a new conversation.
			 */
			struct tm *tm;
			time_t stamp;
			PurpleLog *log;
			data = g_new0(struct msn_logger_data, 1);
			data->root = root;
			data->message = message;
			data->session_id = session_id;
			data->text = NULL;
			data->last_log = FALSE;
			stamp = msn_logger_parse_timestamp(message, &tm);
			log = purple_log_new(PURPLE_LOG_IM, sn, account, NULL, stamp, tm);
			log->logger = msn_logger;
			log->logger_data = data;
			list = g_list_prepend(list, log);
		}
		old_session_id = session_id;
	}
	if (data)
		data->last_log = TRUE;
	return g_list_reverse(list);
}
