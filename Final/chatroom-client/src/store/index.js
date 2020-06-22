import Vue from 'vue'
import Vuex from 'vuex'

// import example from './module-example'

Vue.use(Vuex)

/*
 * If not building with SSR mode, you can
 * directly export the Store instantiation;
 *
 * The function below can be async too; either use
 * async/await or return a Promise which resolves
 * with the Store instance.
 */

export default function(/* { ssrContext } */) {
  const Store = new Vuex.Store({
    state: {
      userInfo: null
    },
    getters: {
      getUserInfo: state => state.userInfo
    },
    mutations: {
      login(state, userInfo) {
        state.userInfo = userInfo
      },
      logout(state) {
        state.userInfo = null
        state.rsaSk = ''
        state.dsaSk = ''
      }
    },
    actions: {
      Login({ commit, state }, userInfo) {
        commit('login', userInfo)
        sessionStorage.setItem('store', JSON.stringify(state))
      },
      Logout({ commit }) {
        commit('logout')
        sessionStorage.removeItem('store')
      }
    },

    // enable strict mode (adds overhead!)
    // for dev mode only
    strict: process.env.DEV
  })

  return Store
}
