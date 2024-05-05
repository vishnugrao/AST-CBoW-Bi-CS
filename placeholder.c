gboolean
purple_certificate_register_scheme(PurpleCertificateScheme *scheme)
{
	g_return_val_if_fail(scheme != NULL, FALSE);
	/* Make sure no scheme is registered with the same name */
	if (purple_certificate_find_scheme(scheme->name) != NULL) {
		return FALSE;
	}
	/* Okay, we're golden. Register it. */
	cert_schemes = g_list_prepend(cert_schemes, scheme);
	/* TODO: Signalling and such? */
	purple_debug_info("certificate",
			  "CertificateScheme %s registered\n",
			  scheme->name);
	return TRUE;
}
