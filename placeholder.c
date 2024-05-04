void
purple_marshal_VOID__POINTER_POINTER_POINTER_POINTER_UINT(PurpleCallback cb,
													    va_list args,
													    void *data,
													    void **return_val)
{
	void *arg1 = va_arg(args, void *);
	void *arg2 = va_arg(args, void *);
	void *arg3 = va_arg(args, void *);
	void *arg4 = va_arg(args, void *);
	guint arg5 = va_arg(args, guint);
	((void (*)(void *, void *, void *, void *, guint, void *))cb)(arg1, arg2, arg3, arg4, arg5, data);
}
