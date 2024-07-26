#include "quickjs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// C++ 函数
int add(int a, int b)
{
    return a + b;
}

// C 封装函数
static JSValue add_wrapper(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv)
{
    if (argc != 2) {
        return JS_ThrowTypeError(ctx, "add function requires exactly two arguments");
    }

    int a = 0;
    JS_ToInt32(ctx, &a, argv[0]);
    int b = 0;
    JS_ToInt32(ctx, &b, argv[1]);

    int result = add(a, b);

    return JS_NewInt32(ctx, result);
}

static JSValue print_wrapper(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv)
{
    for (int i = 0; i < argc; i++) {
        const char* str = JS_ToCString(ctx, argv[i]);
        printf("%s", str);
        JS_FreeCString(ctx, str);
    }
    printf("\n");
    return JS_TRUE;
}

int main()
{
    // 初始化 QuickJS 运行时和上下文
    JSRuntime* rt = JS_NewRuntime();
    JSContext* ctx = JS_NewContext(rt);

    // 创建一个全局对象（如果尚未存在）
    JSValue global_obj = JS_GetGlobalObject(ctx);

    // 将 C 函数添加到全局对象
    JSCFunction func;
    JSValue add_func_obj = JS_NewCFunction(ctx, add_wrapper, "add", 2);
    JS_SetPropertyStr(ctx, global_obj, "add", add_func_obj);
    JSValue print_func_obj = JS_NewCFunction(ctx, print_wrapper, "print", 2);
    JS_SetPropertyStr(ctx, global_obj, "print", print_func_obj);

    JS_FreeValue(ctx, global_obj);
    // global.add = function (a, b) { return a + b };

    // 执行 JavaScript 代码
    // const char* js_code = "console.log(add(3, 4));";

    // 注意：QuickJS 没有内置的 console.log，我们需要自己实现或使用其他方式输出
    // 这里我们假设你已经有了一个 print 函数，或者我们将直接执行并检查返回值

    // 由于 QuickJS 没有 console.log，我们可以修改 JavaScript 代码来直接返回结果
    const char* js_code_modified = "var result = add(3, 4); print(result);";

    // 注意：这里我们假设已经有一个全局的 print 函数
    // 在实际中，你可能需要像这样定义它：
    // JS_DefineFunction(ctx, JS_GetGlobalObject(ctx), "print", print_wrapper, 1, JS_CFUNC_generic);
    // ... 然后实现 print_wrapper ...

    // 但为了简化，我们将直接检查 add 函数的返回值
    JSValue result = JS_Eval(ctx, js_code_modified, strlen(js_code_modified), "<input>", JS_EVAL_TYPE_GLOBAL);
    if (JS_IsException(result)) {
        // 处理异常
        fprintf(stderr, "JavaScript error: %s\n", JS_ToCString(ctx, JS_GetException(ctx)));
        JS_FreeValue(ctx, result);
    } else {
        // 输出结果
        int32_t int_result = 0;
        JS_ToInt32(ctx, &int_result, result);
        printf("Result: %d\n", int_result);
    }

    // 清理
    JS_FreeValue(ctx, result);
    JS_FreeContext(ctx);
    JS_FreeRuntime(rt);

    return 0;
}