// LitWare Menu Controller
class MenuController {
    constructor() {
        this.currentTab = 'aim';
        this.listeningForKey = null;
        this.config = {};

        this.initializeTabs();
        this.initializeControls();
        this.initializeListeners();
    }

    initializeTabs() {
        const navBtns = document.querySelectorAll('.nav-btn');
        const tabContents = document.querySelectorAll('.tab-content');
        const indicator = document.querySelector('.sidebar-indicator');

        navBtns.forEach((btn, index) => {
            btn.addEventListener('click', (e) => {
                const tabName = btn.getAttribute('data-tab');

                // Update active states
                navBtns.forEach(b => b.classList.remove('active'));
                tabContents.forEach(t => t.classList.remove('active'));

                btn.classList.add('active');
                document.querySelector(`[data-tab="${tabName}"]`).classList.add('active');

                // Move indicator
                const offset = index * 34 + 10;
                indicator.style.transform = `translateY(${offset}px)`;

                this.currentTab = tabName;
                this.sendMessage('tab_changed', { tab: tabName });
            });
        });
    }

    initializeControls() {
        // Checkboxes
        document.querySelectorAll('input[type="checkbox"]').forEach(input => {
            input.addEventListener('change', (e) => {
                this.handleControlChange(e.target);
            });
        });

        // Sliders
        document.querySelectorAll('.slider').forEach(slider => {
            slider.addEventListener('input', (e) => {
                this.handleSliderChange(e.target);
            });
        });

        // Keybind buttons
        document.querySelectorAll('.keybind-btn').forEach(btn => {
            btn.addEventListener('click', (e) => {
                this.startKeyCapture(e.target);
            });
        });

        // Select controls
        document.querySelectorAll('.select-control').forEach(select => {
            select.addEventListener('change', (e) => {
                this.handleControlChange(e.target);
            });
        });

        // Color pickers
        document.querySelectorAll('.color-picker').forEach(picker => {
            picker.addEventListener('change', (e) => {
                this.handleControlChange(e.target);
            });
        });

        // Theme buttons
        document.querySelectorAll('.theme-btn').forEach(btn => {
            btn.addEventListener('click', (e) => {
                document.querySelectorAll('.theme-btn').forEach(b => b.classList.remove('active'));
                btn.classList.add('active');
                const theme = btn.getAttribute('data-theme');
                this.applyTheme(theme);
            });
        });

        // Config buttons
        document.getElementById('config-save').addEventListener('click', () => this.saveConfig());
        document.getElementById('config-load').addEventListener('click', () => this.loadConfig());
        document.getElementById('config-reset').addEventListener('click', () => this.resetConfig());
    }

    initializeListeners() {
        // Update slider values in real-time
        document.querySelectorAll('.slider').forEach(slider => {
            const displayId = slider.id + '-val';
            const display = document.getElementById(displayId);
            if (display) {
                slider.addEventListener('input', () => {
                    const value = parseFloat(slider.value);
                    display.textContent = isNaN(value) ? '0' : value.toFixed(parseFloat(slider.step || 1) === 1 ? 0 : 2);
                });
            }
        });

        // Listen for messages from C++
        if (window.chromeBridge) {
            window.chromeBridge.onMessage = (message) => {
                this.handleCppMessage(message);
            };
        }
    }

    handleControlChange(element) {
        const id = element.id;
        const value = element.type === 'checkbox' ? element.checked : element.value;

        this.sendMessage('control_changed', {
            id: id,
            value: value
        });

        // Update slider backgrounds
        if (element.classList.contains('slider')) {
            const percentage = (element.value - element.min) / (element.max - element.min) * 100;
            element.style.backgroundSize = `${percentage}% 100%`;
        }
    }

    handleSliderChange(slider) {
        const percentage = (slider.value - slider.min) / (slider.max - slider.min) * 100;
        slider.style.backgroundSize = `${percentage}% 100%`;
        this.handleControlChange(slider);
    }

    startKeyCapture(button) {
        if (this.listeningForKey === button) {
            this.stopKeyCapture();
            return;
        }

        // Stop previous listener
        if (this.listeningForKey) {
            this.stopKeyCapture();
        }

        this.listeningForKey = button;
        button.classList.add('listening');
        button.textContent = 'Press Key...';

        const handleKeyDown = (e) => {
            e.preventDefault();
            const keyName = this.getKeyName(e);
            button.textContent = keyName;

            this.sendMessage('keybind_set', {
                id: button.id,
                key: e.key,
                code: e.code
            });

            this.stopKeyCapture();
        };

        window.addEventListener('keydown', handleKeyDown, { once: true });
    }

    stopKeyCapture() {
        if (this.listeningForKey) {
            this.listeningForKey.classList.remove('listening');
            this.listeningForKey = null;
        }
    }

    getKeyName(event) {
        const keyNames = {
            'ArrowUp': 'Up', 'ArrowDown': 'Down', 'ArrowLeft': 'Left', 'ArrowRight': 'Right',
            'Enter': 'Enter', 'Escape': 'ESC', ' ': 'Space', 'Shift': 'Shift',
            'Control': 'Ctrl', 'Alt': 'Alt', 'Meta': 'Win'
        };
        return keyNames[event.key] || event.key.toUpperCase();
    }

    sendMessage(type, data = {}) {
        const message = { type, ...data };

        // Send to C++ through CEF
        if (window.chromeBridge && window.chromeBridge.send) {
            window.chromeBridge.send(JSON.stringify(message));
        } else {
            console.log('[Message to C++]', message);
        }
    }

    handleCppMessage(message) {
        const data = typeof message === 'string' ? JSON.parse(message) : message;

        switch (data.type) {
            case 'update_stats':
                this.updateStats(data);
                break;
            case 'update_control':
                this.updateControl(data);
                break;
            case 'update_status':
                this.updateStatus(data);
                break;
            case 'notification':
                this.showNotification(data);
                break;
        }
    }

    updateStats(data) {
        if (data.players !== undefined) {
            document.getElementById('stat-players').textContent = `Players: ${data.players}`;
            document.getElementById('status-players').textContent = data.players;
        }
        if (data.fps !== undefined) {
            document.getElementById('stat-fps').textContent = `FPS: ${data.fps}`;
        }
    }

    updateControl(data) {
        const element = document.getElementById(data.id);
        if (element) {
            if (element.type === 'checkbox') {
                element.checked = data.value;
            } else {
                element.value = data.value;
                if (element.classList.contains('slider')) {
                    const percentage = (element.value - element.min) / (element.max - element.min) * 100;
                    element.style.backgroundSize = `${percentage}% 100%`;
                }
            }
        }
    }

    updateStatus(data) {
        if (data.esp !== undefined) {
            document.getElementById('status-esp').textContent = data.esp ? 'ON' : 'OFF';
        }
        if (data.hook !== undefined) {
            document.getElementById('status-hook').textContent = data.hook ? 'ACTIVE' : 'IDLE';
        }
    }

    showNotification(data) {
        const notification = document.createElement('div');
        notification.className = 'notification';
        notification.textContent = data.message;
        notification.style.cssText = `
            position: fixed;
            top: 50%;
            left: 50%;
            transform: translate(-50%, -50%);
            background: rgba(0, 255, 136, 0.9);
            color: #0a0e1a;
            padding: 12px 20px;
            border-radius: 4px;
            z-index: 1000;
            animation: slideIn 0.3s ease;
        `;
        document.body.appendChild(notification);
        setTimeout(() => notification.remove(), 3000);
    }

    saveConfig() {
        const name = document.getElementById('config-name').value || 'default';
        const config = this.collectConfig();

        this.sendMessage('save_config', {
            name: name,
            config: config
        });
    }

    loadConfig() {
        const name = document.getElementById('config-name').value || 'default';
        this.sendMessage('load_config', { name: name });
    }

    resetConfig() {
        if (confirm('Reset all settings to default?')) {
            this.sendMessage('reset_config', {});
        }
    }

    applyTheme(theme) {
        const themes = {
            '0': { // Dark Pro
                '--primary-bg': '#0a0e1a',
                '--secondary-bg': '#11152b',
                '--tertiary-bg': '#1a1f3a',
            },
            '1': { // Glassmorphism
                '--primary-bg': '#0f1419cc',
                '--secondary-bg': '#1a1f2dcc',
                '--tertiary-bg': '#242f42cc',
            },
            '2': { // Neumorphism
                '--primary-bg': '#2d323e',
                '--secondary-bg': '#35394a',
                '--tertiary-bg': '#3f444f',
            }
        };

        const themeColors = themes[theme] || themes['0'];
        Object.entries(themeColors).forEach(([key, value]) => {
            document.documentElement.style.setProperty(key, value);
        });

        this.sendMessage('theme_changed', { theme: parseInt(theme) });
    }

    collectConfig() {
        const config = {};
        document.querySelectorAll('[id]').forEach(element => {
            if (element.id && !element.id.startsWith('stat-') && !element.id.startsWith('status-')) {
                if (element.type === 'checkbox') {
                    config[element.id] = element.checked;
                } else if (element.classList.contains('slider') || element.type === 'range') {
                    config[element.id] = parseFloat(element.value);
                } else if (element.type === 'color') {
                    config[element.id] = element.value;
                } else if (element.tagName === 'SELECT') {
                    config[element.id] = element.value;
                } else if (element.type === 'text') {
                    config[element.id] = element.value;
                }
            }
        });
        return config;
    }
}

// Initialize on page load
document.addEventListener('DOMContentLoaded', () => {
    window.menuController = new MenuController();
    console.log('[LitWare Menu] Initialized');
});

// Handle CEF integration
if (typeof window.cefQuery !== 'undefined') {
    window.chromeBridge = {
        send: (message) => {
            window.cefQuery({
                request: message,
                onSuccess: (response) => {
                    if (response) {
                        window.menuController.handleCppMessage(response);
                    }
                },
                onFailure: (error) => {
                    console.error('CEF Query Error:', error);
                }
            });
        },
        onMessage: null
    };
}
