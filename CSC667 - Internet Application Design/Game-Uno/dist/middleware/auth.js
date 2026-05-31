export function requireAuth(request, response, next) {
    if (request.session.user?.id) {
        next();
    }
    else {
        response.redirect("/auth/login");
    }
}
//# sourceMappingURL=auth.js.map