<template>
  <q-layout view="hHh LpR fFf">
    <q-header>
      <q-bar class="q-electron-drag bg-primary">
        <q-btn
          dense
          flat
          round
          icon="lens"
          size="8.5px"
          color="red"
          @click="closeApp"
        />
        <q-btn
          dense
          flat
          round
          icon="lens"
          size="8.5px"
          color="yellow"
          @click="minimize"
        />
        <q-btn
          dense
          flat
          round
          icon="lens"
          size="8.5px"
          color="green"
          @click="maximize"
        />
        <div class="col text-center text-weight-bold">Chatroom</div>
        <q-space></q-space>
        <q-btn
          dense
          v-if="!$store.getters.getUserInfo"
          flat
          icon="person"
          to="/auth/login"
          >Login</q-btn
        >
        <template v-else>
          <q-btn size="xs" round>
            <q-avatar size="sm" color="red">{{
              $store.getters.getUserInfo.username[0]
            }}</q-avatar>
            <q-menu
              fit
              transition-show="flip-right"
              transition-hide="flip-left"
            >
              <q-list style="min-width:140px">
                <q-item>
                  <q-item-section avatar>
                    <q-avatar size="sm" color="red">{{
                      $store.getters.getUserInfo.username[0]
                    }}</q-avatar>
                  </q-item-section>
                  <q-item-section>{{
                    $store.getters.getUserInfo.username
                  }}</q-item-section>
                </q-item>
                <q-separator></q-separator>
                <q-item clickable v-close-popup @click="logout">
                  <q-item-section avatar>
                    <q-icon color="primary" name="logout"></q-icon>
                  </q-item-section>
                  <q-item-section>Logout</q-item-section>
                </q-item>
              </q-list>
            </q-menu>
          </q-btn>
        </template>
      </q-bar>
    </q-header>

    <q-page-container>
      <transition
        enter-active-class="animated fadeIn"
        leave-active-class="animated fadeOut"
        appear
        duration="300"
      >
        <router-view />
      </transition>
    </q-page-container>
  </q-layout>
</template>

<script>
export default {
  data() {
    return {}
  },
  mounted: function() {
    if (this.$store.getters.getUserInfo) {
    }
  },
  methods: {
    logout() {
      this.$store.dispatch('Logout')
      this.$success('Logout')
      setTimeout(() => {
        this.$router.push('/auth/login')
      }, 500)
    },
    profile() {},
    minimize() {
      if (process.env.MODE === 'electron') {
        this.$q.electron.remote.BrowserWindow.getFocusedWindow().minimize()
      }
    },

    maximize() {
      if (process.env.MODE === 'electron') {
        const win = this.$q.electron.remote.BrowserWindow.getFocusedWindow()

        if (win.isMaximized()) {
          win.unmaximize()
        } else {
          win.maximize()
        }
      }
    },

    closeApp() {
      if (process.env.MODE === 'electron') {
        this.$q.electron.remote.BrowserWindow.getFocusedWindow().close()
      }
    }
  }
}
</script>
