}
int gg_protobuf_valid_chknull(struct gg_session *gs, const char *msg_name,
	int isNull)
{
	if (isNull) {
		gg_debug_session(gs, GG_DEBUG_ERROR, "// gg_protobuf: couldn't "
			"unpack %s message\n", msg_name);
	}
	return !isNull;
}
