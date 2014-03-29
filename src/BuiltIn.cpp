#include <PCH.h>
#include <BuiltIn.h>
#include <Values.h>
#include <AST.h>


map<string const, AbstractValue const*> BuiltInEnvironment::BuildTerms() {
	map<string const, AbstractValue const*> Terms;

	Terms["nil"] = new Closure({ "x", "y" }, new VariableNode("y"), BaseEnvironment());
	Terms["true"] = new Closure({ "x", "y" }, new VariableNode("x"), BaseEnvironment());
	Terms["false"] = new Closure({ "x", "y" }, new VariableNode("y"), BaseEnvironment());

	//Terms["Z"] = new Closure(
	//	{ "f" },
	//	new ApplicationNode({ new FunctionNode(
	//		{ "x" },
	//		new ApplicationNode({ new VariableNode("f"), new FunctionNode(
	//			{ "v" },
	//			new ApplicationNode({ new VariableNode("x"), new VariableNode("x"), new VariableNode("v") })) })), new FunctionNode(
	//		{ "x" },
	//		new ApplicationNode({ new VariableNode("f"), new FunctionNode(
	//			{ "v" },
	//			new ApplicationNode({ new VariableNode("x"), new VariableNode("x"), new VariableNode("v") })) })) }), BaseEnvironment());

	return Terms;
}


BuiltInEnvironment::BuiltInEnvironment()
	:
BaseEnvironment(BuildTerms()) {}
