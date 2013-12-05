namespace IRTree {
	class IExpression {
		virtual void Visit();
	};

	class IStatement{
	};
};

enum TBinop {
	PLUS, MINUS, MUL, DIV, AND, OR, LSHIFT, RSHIFT, ARSHIFT, XOR
};

enum TCJump {
	EQ, NE, LT, GT, LE, GE, ULT, ULE, UGT, UGE
};

class CConst : IRTree::IExpression {
};

class CName : IRTree::IExpression {
};

class CTemp : IRTree::IExpression {
};

class CBinop : IRTree::IExpression {
};

class CMem : IRTree::IExpression {
};

class CCall : IRTree::IExpression {
};

class CEseq : IRTree::IExpression {
};

class CMove : IRTree::IStatement {
};

class CExp : IRTree::IStatement {
};

class CJump : IRTree::IStatement {
};

class CCJump : IRTree::IStatement {
};

class CSeq : IRTree::IStatement {
};

class CLabel : IRTree::IStatement {
};

class CExpList {
};

class CStmList {
};