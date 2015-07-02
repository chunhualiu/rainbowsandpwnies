# Introduction #

Tokens in cold\_fuzz are the expressions we use to dynamically create different output for requests. This tutorial will walk you through the steps of creating your own tokens for cold\_fuzz in C.

For the purposes of this tutorial, we will add an example token called say to the fuzzer. If you want the token say to print out "hello world", it would look like:
```
%type=say;text=hello world;%
```

# Terminology #
  * **expression** - The expression refers to the text `%type=say;text=hello world;%`
  * **token** - Token refers to the internal storage/handling of our expression.
  * **parser** - The parser handles turning the expression into something our token can use
  * **element** - An element is a piece of our expression. For `type=say`;, a corresponding element will be created with `key=type` and `value=say`

# fuzzer.h #

First, we will need to define a new token type and a new token description. The token type must be unique, and the token description will be equal to the value of the element where key=type.

```
#define FUZZER_TOKEN_TYPE_SAY 1000
#define FUZZER_TOKEN_DESCRIPTION_SAY "SAY"
```

_Note, the token description is not case sensitive, but uppercase by convention_

Next, we create a struct to hold the information for our token.

```
typedef struct
{
	char * text;
} fuzzer_token_say_t;
```

Finally, we need three functions. One to create our token, one to execute our token, and one to destroy our token.

```
fuzzer_token_say_t * fuzzer_token_say_create  (fuzzer_token_parser_t * parser);
void                 fuzzer_token_say_destroy (fuzzer_token_say_t * say);
int                  fuzzer_token_say_execute (fuzzer_token_say_t * say, unsigned char ** result, int * length);
```

# fuzzer.c #

We need to add code to three functions, `fuzzer_token_create`, `fuzzer_token_destroy`, and `fuzzer_token_execute` which will call our token's functions when appropriate.

**fuzzer\_token\_create**
```
	else if (fuzzer_case_insensitive_strcmp(element->value, FUZZER_TOKEN_DESCRIPTION_SAY) == 0)
	{
		token->type = FUZZER_TOKEN_TYPE_SAY;
		token->token = (void *) fuzzer_token_say_create(parser);
	}
```

**fuzzer\_token\_destroy**
```
	else if (token->type == FUZZER_TOKEN_TYPE_SAY)
		fuzzer_token_say_destroy((fuzzer_token_say_t *) token->token);
```

**fuzzer\_token\_execute**
```
	else if (token->type == FUZZER_TOKEN_TYPE_SAY)
		error = fuzzer_token_say_execute ((fuzzer_token_say_t *) token->token, &result, &length);
```

Finally, we need to create our token's three functions. You may want to create some custom error codes, but for the purpose of this tutorial we will use an error of -1 for all errors.

Some people don't believe in checking for malloc fails. Because cold\_fuzz can quickly use a massive amount of memory depending on what the user does, I like to return something. That being said, I don't always clean up if a malloc fails :/.

```
fuzzer_token_say_t * fuzzer_token_say_create (fuzzer_token_parser_t * parser)
{

	fuzzer_token_say_t * say;
	fuzzer_token_parser_element_t * text;
	
	say = (fuzzer_token_say_t *) malloc(sizeof(fuzzer_token_say_t));
	if (say == NULL)
	{
		fuzzer_errno = -1;
		return NULL;
	}
	say->text = NULL; // if the next malloc fails and someone tries fuzzer_token_ascii_destroy, we won't die on free()
	
	text = fuzzer_token_parser_get_element_by_key(parser, "text");
	if (text == NULL)
	{
		fuzzer_errno = -1;
		return NULL;
	}
	
	say->text = (char *) malloc(strlen(text->value) + 1);
	memcpy(say->text, text->value, strlen(text->value));
	say->text[strlen(text->value)] = (char) 0;
	
	return say;
	
}
```

Some clean up for when the fuzzer engine is done with our token.

```
void fuzzer_token_say_destroy (fuzzer_token_say_t * say)
{
	if (say == NULL)
		return;
	if (say->text != NULL)
		free(say->text);
	free (say);
	
}
```

fuzzer\_token\_say\_execute is called everytime the fuzzer\_engine needs it to generate text. It allocates result to the size needed, sets length to the number of bytes in result, and returns 0 on success. Note that if you return an error (non-zero value), result will not be freed. Also note that result is NOT to be null terminated, and length should not account for a null byte.

```
int fuzzer_token_say_execute (fuzzer_token_say_t * say, unsigned char ** result, int * length)
{

	*length = strlen(say->text);
	
	*result = (unsigned char *) malloc(*length);
	if (*result == NULL)
		return -1;
	
	memcpy(*result, say->text, *length);
	
	return 0;
	
}
```

That's it. You should be done. The code in this tutorial is _untested_, but _should_ work just fine.