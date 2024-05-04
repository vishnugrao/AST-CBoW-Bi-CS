};
static int function_fieldnum_helper(struct ast_channel *chan, const char *cmd,
				char *parse, char *buf, struct ast_str **sbuf, ssize_t len)
{
	char *varsubst, *field;
	struct ast_str *str = ast_str_thread_get(&result_buf, 16);
	int fieldindex = 0, res = 0;
	AST_DECLARE_APP_ARGS(args,
		AST_APP_ARG(varname);
		AST_APP_ARG(delim);
		AST_APP_ARG(field);
	);
	char delim[2] = "";
	size_t delim_used;
	if (!str) {
		return -1;
	}
	AST_STANDARD_APP_ARGS(args, parse);
	if (args.argc < 3) {
		ast_log(LOG_ERROR, "Usage: FIELDNUM(<listname>,<delimiter>,<fieldvalue>)\n");
		res = -1;
	} else {
		varsubst = ast_alloca(strlen(args.varname) + 4);
		sprintf(varsubst, "${%s}", args.varname);
		ast_str_substitute_variables(&str, 0, chan, varsubst);
		if (ast_str_strlen(str) == 0 || ast_strlen_zero(args.delim)) {
			fieldindex = 0;
		} else if (ast_get_encoded_char(args.delim, delim, &delim_used) == -1) {
			res = -1;
		} else {
			char *varval = ast_str_buffer(str);
			while ((field = strsep(&varval, delim)) != NULL) {
				fieldindex++;
				if (!strcasecmp(field, args.field)) {
					break;
				}
			}
			if (!field) {
				fieldindex = 0;
			}
			res = 0;
		}
	}
	if (sbuf) {
		ast_str_set(sbuf, len, "%d", fieldindex);
	} else {
		snprintf(buf, len, "%d", fieldindex);
	}
	return res;
}
