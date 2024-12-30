'use strict';

const path = require('path');

// 加载编译后的插件
let addon = null;
try {
    // 首先尝试加载 Release 版本
    addon = require('./build/Release/addon.node');
} catch (err) {
    try {
        // 如果 Release 版本不存在，尝试加载 Debug 版本
        addon = require('./build/Debug/addon.node');
    } catch (err2) {
        throw new Error('Failed to load the native addon: ' + err2.message);
    }
}

// 包装原生方法，添加错误处理
class AddonWrapper {
    constructor() {
        // 确保原生模块已正确加载
        if (!addon) {
            throw new Error('Native addon not loaded');
        }
    }

    // 包装同步方法示例
    init() {
        try {
            return addon.init();
        } catch (err) {
            throw new Error(`Failed to call hello: ${err.message}`);
        }
    }

    getMachineCode() {
        try {
            return addon.getMachineCode();
        } catch (err) {
            throw new Error(`Failed to call hello: ${err.message}`);
        }
    }

    detect(img, w, h) {
        try {
            return addon.detect(img, w, h);
        } catch (err) {
            throw new Error(`Failed to call hello: ${err.message}`);
        }
    }

    detectAsync(img, w, h, callback) {
        try {
            return addon.detectAsync(img, w, h, callback);
        } catch (err) {
            throw new Error(`Failed to call hello: ${err.message}`);
        }
    }

    enableGpu(state) {
        try {
            return addon.enableGpu(state);
        } catch (err) {
            throw new Error(`Failed to call hello: ${err.message}`);
        }
    }

    enableDebug(state) {
        try {
            return addon.enableDebug(state);
        } catch (err) {
            throw new Error(`Failed to call hello: ${err.message}`);
        }
    }
}

// 创建单例实例
const instance = new AddonWrapper();

// 导出实例方法
module.exports = {
    init: (...args) => instance.init(...args),
    getMachineCode: (...args) => instance.getMachineCode(...args),
    detect: (img, w, h) => instance.detect(img, w, h),
    detectAsync: (img, w, h, callback) => instance.detectAsync(img, w, h, callback),
    enableGpu: (state) => instance.enableGpu(state || false),
    enableDebug: () => instance.enableDebug(),

    // 导出原始addon对象，以防高级用户需要直接访问
    _raw: addon
};
