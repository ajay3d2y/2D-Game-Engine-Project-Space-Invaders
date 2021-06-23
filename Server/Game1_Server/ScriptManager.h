#pragma once
#include <dukglue/dukglue.h>
#include<fstream>
#include<streambuf>

class ScriptManager {
public:
	ScriptManager() {
		ctx = duk_create_heap_default();
		if (!ctx) {
			printf("Failed\n");
		}
		else {
			printf("Successful\n");
		}
		registerMethod();
	}

	void registerMethod() {
		dukglue_register_constructor<Enemy>(ctx, "Enemy");
		dukglue_register_method(ctx, &Enemy::getX, "x");
		dukglue_register_method(ctx, &Enemy::getY, "y");
		dukglue_register_method(ctx, &Enemy::getOffset, "offSet");
		dukglue_register_method(ctx, &Enemy::getDirection, "direction");
		dukglue_register_method(ctx, &Enemy::setPosition, "setPos");
		dukglue_register_method(ctx, &Enemy::changeDirection, "chngDirection");

		duk_push_c_function(ctx, native_print, DUK_VARARGS);
		duk_put_global_string(ctx, "print");


	}

	static duk_ret_t native_print(duk_context* ctx)
	{
		duk_push_string(ctx, " ");
		duk_insert(ctx, 0);
		duk_join(ctx, duk_get_top(ctx) /* #args -> need to do this if we pass DUK_VARARGS when pushing this c_function */
			- 1);
		printf("%s\n", duk_safe_to_string(ctx, -1));
		return 0;
	}



	void setupScript(const char* script) {
		loadScript(ctx, script);
		duk_peval(ctx);
		duk_pop(ctx);


	}

	void loadScript(duk_context* ctx, const char* script) {
		std::ifstream tmp(script);
		std::stringstream buffer;
		buffer << tmp.rdbuf();
		duk_push_lstring(ctx, buffer.str().c_str(), (duk_size_t)(buffer.str().length()));
	}

	void moveEnemy(Enemy* e) {
		duk_push_global_object(ctx);
		duk_get_prop_string(ctx, -1, "movEnemy");
		dukglue_push(ctx, e);
		if (duk_pcall(ctx, 1) != 0)
			printf("Error: %s\n", duk_safe_to_string(ctx, -1));
		duk_pop(ctx);
	}
	duk_context* ctx = NULL;
};
