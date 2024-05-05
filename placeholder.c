}
void ast_channel_set_rawwriteformat(struct ast_channel *chan, struct ast_format *format)
{
	ao2_replace(chan->rawwriteformat, format);
}
