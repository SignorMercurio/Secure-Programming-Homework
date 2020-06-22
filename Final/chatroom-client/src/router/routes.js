const routes = [
  {
    path: '/',
    redirect: '/auth/login',
    component: () => import('layouts/HeaderOnly.vue'),
    children: [
      {
        path: 'chat',
        component: () => import('pages/Chat.vue'),
        meta: { auth: true }
      }
    ]
  },
  {
    path: '/auth',
    component: () => import('layouts/HeaderOnly.vue'),
    children: [
      { path: 'login', component: () => import('pages/auth/Login.vue') },
      { path: 'register', component: () => import('pages/auth/Register.vue') }
    ]
  }
]

// Always leave this as last one
if (process.env.MODE !== 'ssr') {
  routes.push({
    path: '*',
    component: () => import('pages/Error404.vue')
  })
}

export default routes
