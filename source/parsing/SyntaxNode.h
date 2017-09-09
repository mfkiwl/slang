//------------------------------------------------------------------------------
// SyntaxNode.h
// Base class and utilities for syntax nodes.
//
// File is under the MIT license; see LICENSE for details.
//------------------------------------------------------------------------------
#pragma once

#include <cmath>
#include <cstdint>
#include <cstddef>
#include <string>

#include "span.h"
#include "lexing/Token.h"
#include "util/SmallVector.h"

namespace slang {

class SyntaxNode;

enum class SyntaxKind : uint16_t {
    Unknown,
    List,

    // directives
    BeginKeywordsDirective,
    CellDefineDirective,
    DefaultNetTypeDirective,
    DefineDirective,
    ElseDirective,
    ElsIfDirective,
    EndKeywordsDirective,
    EndCellDefineDirective,
    EndIfDirective,
    IfDefDirective,
    IfNDefDirective,
    IncludeDirective,
    LineDirective,
    NoUnconnectedDriveDirective,
    PragmaDirective,
    ResetAllDirective,
    TimescaleDirective,
    UnconnectedDriveDirective,
    UndefDirective,
    UndefineAllDirective,

    // macros
    MacroUsage,
    MacroFormalArgumentList,
    MacroFormalArgument,
    MacroArgumentDefault,
    MacroActualArgumentList,
    MacroActualArgument,

    // attributes
    AttributeSpec,
    AttributeInstance,

    // arguments
    EmptyArgument,
    OrderedArgument,
    NamedArgument,
    ArgumentList,
    ParameterValueAssignment,

    // patterns
    VariablePattern,
    WildcardPattern,
    ExpressionPattern,
    TaggedPattern,
    OrderedStructurePatternMember,
    NamedStructurePatternMember,
    StructurePattern,
    MatchesClause,
    ConditionalPattern,
    ConditionalPredicate,
    SimpleAssignmentPattern,
    AssignmentPatternItem,
    StructuredAssignmentPattern,
    ReplicatedAssignmentPattern,

    // misc expressions
    TaggedUnionExpression,
    OpenRangeList,
    InsideExpression,
    ConditionalExpression,
    ExpressionOrDist,
    BadExpression,

    // unary expressions
    UnaryPlusExpression,
    UnaryMinusExpression,
    UnaryBitwiseAndExpression,
    UnaryBitwiseNandExpression,
    UnaryBitwiseOrExpression,
    UnaryBitwiseNorExpression,
    UnaryBitwiseXorExpression,
    UnaryBitwiseXnorExpression,
    UnaryPreincrementExpression,
    UnaryPredecrementExpression,
    UnaryLogicalNotExpression,
    UnaryBitwiseNotExpression,
    UnarySequenceDelayExpression,
    UnarySequenceEventExpression,
    UnaryNotPropertyExpression,
    AcceptOnPropertyExpression,
    RejectOnPropertyExpression,
    SyncAcceptOnPropertyExpression,
    SyncRejectOnPropertyExpression,
    NextTimePropertyExpression,
    SNextTimePropertyExpression,
    AlwaysPropertyExpression,
    SAlwaysPropertyExpression,
    EventuallyPropertyExpression,
    SEventuallyPropertyExpression,

    // primary expressions
    NullLiteralExpression,
    StringLiteralExpression,
    IntegerLiteralExpression,
    IntegerVectorExpression,
    UnbasedUnsizedLiteralExpression,
    RealLiteralExpression,
    TimeLiteralExpression,
    WildcardLiteralExpression,
    OneStepLiteralExpression,
    ParenthesizedExpression,
    MinTypMaxExpression,
    EmptyQueueExpression,
    ConcatenationExpression,
    MultipleConcatenationExpression,
    StreamingConcatenationExpression,
    StreamExpression,
    StreamExpressionWithRange,
    NewClassExpression,
    NewArrayExpression,
    AssignmentPatternExpression,
    DefaultPatternKeyExpression,
    NewExpression,

    // selectors
    BitSelect,
    SimpleRangeSelect,
    AscendingRangeSelect,
    DescendingRangeSelect,
    ElementSelect,

    // postfix expressions
    ElementSelectExpression,
    MemberAccessExpression,
    InvocationExpression,
    PostincrementExpression,
    PostdecrementExpression,
    CastExpression,
    SignedCastExpression,
    WithClause,
    IdentifierList,
    RandomizeMethodWithClause,

    // binary expressions
    AddExpression,
    SubtractExpression,
    MultiplyExpression,
    DivideExpression,
    PowerExpression,
    ModExpression,
    EqualityExpression,
    InequalityExpression,
    CaseEqualityExpression,
    CaseInequalityExpression,
    WildcardEqualityExpression,
    WildcardInequalityExpression,
    LessThanExpression,
    LessThanEqualExpression,
    GreaterThanExpression,
    GreaterThanEqualExpression,
    LogicalAndExpression,
    LogicalOrExpression,
    BinaryAndExpression,
    BinaryOrExpression,
    BinaryXorExpression,
    BinaryXnorExpression,
    LogicalImplicationExpression,
    LogicalEquivalenceExpression,
    LogicalShiftLeftExpression,
    LogicalShiftRightExpression,
    ArithmeticShiftLeftExpression,
    ArithmeticShiftRightExpression,
    BinarySequenceDelayExpression,
    OrSequenceExpression,
    AndSequenceExpression,
    IntersectSequenceExpression,
    WithinSequenceExpression,
    ThroughoutSequenceExpression,
    IffPropertyExpression,
    UntilPropertyExpression,
    SUntilPropertyExpression,
    UntilWithPropertyExpression,
    SUntilWithPropertyExpression,
    ImpliesPropertyExpression,
    OverlappedImplicationPropertyExpression,
    NonOverlappedImplicationPropertyExpression,
    OverlappedFollowedByPropertyExpression,
    NonOverlappedFollowedByPropertyExpression,

    // assignment expressions
    AssignmentExpression,
    AddAssignmentExpression,
    SubtractAssignmentExpression,
    MultiplyAssignmentExpression,
    DivideAssignmentExpression,
    ModAssignmentExpression,
    AndAssignmentExpression,
    OrAssignmentExpression,
    XorAssignmentExpression,
    LogicalLeftShiftAssignmentExpression,
    LogicalRightShiftAssignmentExpression,
    ArithmeticLeftShiftAssignmentExpression,
    ArithmeticRightShiftAssignmentExpression,
    NonblockingAssignmentExpression,

    // names
    LocalScope,
    UnitScope,
    RootScope,
    IdentifierName,
    IdentifierSelectName,
    EmptyIdentifierName,
    ClassName,
    ScopedName,
    SystemName,
    ThisHandle,
    SuperHandle,
    ArrayUniqueMethod,
    ArrayAndMethod,
    ArrayOrMethod,
    ArrayXorMethod,
    ClassScope,
    ConstructorName,

    // timing control
    DelayControl,
    CycleDelay,
    EventControl,
    SignalEventExpression,
    BinaryEventExpression,
    ParenthesizedEventExpression,
    ImplicitEventControl,
    ParenImplicitEventControl,
    EventControlWithExpression,
    RepeatedEventControl,
    TimingControlExpression,
    TimingControlExpressionConcatenation,
    ShortcutCycleDelayRange,

    // declarations
    RangeDimensionSpecifier,
    DataTypeDimensionSpecifier,
    WildcardDimensionSpecifier,
    ColonExpressionClause,
    QueueDimensionSpecifier,
    VariableDimension,
    EqualsValueClause,
    VariableDeclarator,
    DataDeclaration,
    TypedefDeclaration,
    TypedefKeywordDeclaration,
    TypedefInterfaceClassDeclaration,
    PackageImportItem,
    PackageImportDeclaration,
    ParameterDeclaration,
    ParameterDeclarationStatement,
    ChargeStrength,
    DriveStrength,
    NetDeclaration,
    PortDeclaration,
    GenvarDeclaration,

    // types
    BitType,
    LogicType,
    RegType,
    ByteType,
    ShortIntType,
    IntType,
    LongIntType,
    IntegerType,
    TimeType,
    ShortRealType,
    RealType,
    RealTimeType,
    StructType,
    UnionType,
    EnumType,
    StringType,
    CHandleType,
    VirtualInterfaceType,
    NamedType,
    EventType,
    VoidType,
    TypeType,
    ImplicitType,
    TypeReference,
    StructUnionMember,
    DotMemberClause,
    Untyped,
    PropertyType,
    SequenceType,
    VarDataType,

    // statements
    NamedLabel,
    EmptyStatement,
    ElseClause,
    ConditionalStatement,
    DefaultCaseItem,
    PatternCaseItem,
    StandardCaseItem,
    CaseStatement,
    ForeverStatement,
    LoopStatement,
    DoWhileStatement,
    ForVariableDeclaration,
    ForLoopStatement,
    ForeachLoopList,
    ForeachLoopStatement,
    ReturnStatement,
    JumpStatement,
    TimingControlStatement,
    ExpressionStatement,
    ProceduralAssignStatement,
    ProceduralForceStatement,
    ProceduralDeassignStatement,
    ProceduralReleaseStatement,
    DisableStatement,
    DisableForkStatement,
    NamedBlockClause,
    SequentialBlockStatement,
    ParallelBlockStatement,
    WaitStatement,
    WaitForkStatement,
    WaitOrderStatement,
    RandCaseItem,
    RandCaseStatement,
    BlockingEventTriggerStatement,
    NonblockingEventTriggerStatement,

    // assertions
    DeferredAssertion,
    ConcurrentAssertionMember,
    ImmediateAssertionMember,
    ActionBlock,
    ImmediateAssertStatement,
    ImmediateAssumeStatement,
    ImmediateCoverStatement,
    DisableIff,
    PropertySpec,
    AssertPropertyStatement,
    AssumePropertyStatement,
    CoverPropertyStatement,
    CoverSequenceStatement,
    RestrictPropertyStatement,
    ExpectPropertyStatement,

    // modules
    ImplicitNonAnsiPort,
    ExplicitNonAnsiPort,
    NonAnsiPortList,
    InterfacePortHeader,
    VariablePortHeader,
    InterconnectPortHeader,
    NetPortHeader,
    ImplicitAnsiPort,
    ExplicitAnsiPort,
    AnsiPortList,
    WildcardPortList,
    ParameterPortList,
    ModuleHeader,
    ModuleDeclaration,
    InterfaceHeader,
    InterfaceDeclaration,
    ProgramHeader,
    ProgramDeclaration,
    PackageHeader,
    PackageDeclaration,
    ExternModule,

    // members
    EmptyMember,
    InitialBlock,
    FinalBlock,
    AlwaysBlock,
    AlwaysFFBlock,
    AlwaysCombBlock,
    AlwaysLatchBlock,
    GenerateRegion,
    LoopGenerate,
    IfGenerate,
    CaseGenerate,
    GenerateBlock,
    DividerClause,
    TimeUnitsDeclaration,
    OrderedPortConnection,
    NamedPortConnection,
    WildcardPortConnection,
    HierarchicalInstance,
    HierarchyInstantiation,
    FunctionPort,
    FunctionPortList,
    FunctionPrototype,
    FunctionDeclaration,
    AssertionItemPort,
    AssertionItemPortList,
    LetDeclaration,
    PropertyDeclaration,
    SequenceDeclaration,
    TaskDeclaration,
    ExtendsClause,
    ImplementsClause,
    ClassDeclaration,
    ClassPropertyDeclaration,
    ClassMethodDeclaration,
    ClassMethodPrototype,
    ContinuousAssign,
    DefParamAssignment,
    DefParam,
    ModportItem,
    ModportDeclaration,
    ClockingSkew,
    ClockingDirection,
    ClockingItem,
    ClockingDeclaration,
    DPIImportExport,

    // constraints
    DistWeight,
    DistItem,
    DistConstraintList,
    ExpressionConstraint,
    UniquenessConstraint,
    ImplicationConstraint,
    ElseConstraintClause,
    ConditionalConstraint,
    LoopConstraint,
    DisableConstraint,
    SolveBeforeConstraint,
    ConstraintBlock,
    ConstraintPrototype,
    ConstraintDeclaration,

    // covergroups
    WithFunctionSample,
    BinaryBlockEventExpression,
    PrimaryBlockEventExpression,
    BlockCoverageEvent,
    CovergroupDeclaration,
    CoverageOption,
    Coverpoint,
    TransRepeatRange,
    TransRange,
    TransSet,
    DefaultCoverageBinInitializer,
    ExpressionCoverageBinInitializer,
    RangeCoverageBinInitializer,
    TransListCoverageBinInitializer,
    IffClause,
    CoverageBins,

    // top level
    CompilationUnit
};

enum class TokenKind : uint16_t;

SyntaxKind getUnaryPrefixExpression(TokenKind kind);
SyntaxKind getUnaryPostfixExpression(TokenKind kind);
SyntaxKind getLiteralExpression(TokenKind kind);
SyntaxKind getBinaryExpression(TokenKind kind);
SyntaxKind getKeywordNameExpression(TokenKind kind);
SyntaxKind getIntegerType(TokenKind kind);
SyntaxKind getKeywordType(TokenKind kind);
SyntaxKind getProceduralBlockKind(TokenKind kind);
SyntaxKind getModuleDeclarationKind(TokenKind kind);
SyntaxKind getModuleHeaderKind(TokenKind kind);
TokenKind getModuleEndKind(TokenKind kind);
int getPrecedence(SyntaxKind kind);
bool isRightAssociative(SyntaxKind kind);
bool isPossibleDataType(TokenKind kind);
bool isPossibleExpression(TokenKind kind);
bool isPossibleStatement(TokenKind kind);
bool isNetType(TokenKind kind);
bool isPortDirection(TokenKind kind);
bool isPossibleArgument(TokenKind kind);
bool isComma(TokenKind kind);
bool isSemicolon(TokenKind kind);
bool isCloseBrace(TokenKind kind);
bool isIdentifierOrComma(TokenKind kind);
bool isPossibleExpressionOrComma(TokenKind kind);
bool isPossibleExpressionOrCommaOrDefault(TokenKind kind);
bool isPossibleExpressionOrTripleAnd(TokenKind kind);
bool isPossibleOpenRangeElement(TokenKind kind);
bool isPossiblePattern(TokenKind kind);
bool isPossibleDelayOrEventControl(TokenKind kind);
bool isEndKeyword(TokenKind kind);
bool isDeclarationModifier(TokenKind kind);
bool isMemberQualifier(TokenKind kind);
bool isEndOfParenList(TokenKind kind);
bool isEndOfBracedList(TokenKind kind);
bool isEndOfBracketedList(TokenKind kind);
bool isEndOfCaseItem(TokenKind kind);
bool isEndOfConditionalPredicate(TokenKind kind);
bool isEndOfAttribute(TokenKind kind);
bool isEndOfParameterList(TokenKind kind);
bool isEndOfTransSet(TokenKind kind);
bool isNotInType(TokenKind kind);
bool isNotInPortReference(TokenKind kind);
bool isNotInConcatenationExpr(TokenKind kind);
bool isNotInParameterList(TokenKind kind);
bool isPossiblePropertyPortItem(TokenKind kind);
bool isPossibleAnsiPort(TokenKind kind);
bool isPossibleNonAnsiPort(TokenKind kind);
bool isPossibleFunctionPort(TokenKind kind);
bool isPossibleParameter(TokenKind kind);
bool isPossiblePortConnection(TokenKind kind);
bool isPossibleVectorDigit(TokenKind kind);
bool isPossibleLetPortItem(TokenKind kind);
bool isPossibleTransSet(TokenKind kind);
bool isBeforeOrSemicolon(TokenKind kind);
bool isStatement(SyntaxKind kind);
bool isExpression(SyntaxKind kind);

// discriminated union of Token and SyntaxNode
struct TokenOrSyntax {
    union {
        Token token;
        const SyntaxNode* node;
    };
    bool isToken;

    TokenOrSyntax(Token token) : token(token), isToken(true) {}
    TokenOrSyntax(const SyntaxNode* node) : node(node), isToken(false) {}
    TokenOrSyntax(std::nullptr_t) : token(), isToken(true) {}
};

/// Base class for all syntax nodes.
class SyntaxNode {
public:
    /// The kind of syntax node.
    SyntaxKind kind;

    SyntaxNode(SyntaxKind kind) : kind(kind) {}

    /// Utility method to wrap writeTo and generate an std::string.
    std::string toString(uint8_t flags = 0) const;

    /// Write the node and all of its children to a string.
    void writeTo(SmallVector<char>& buffer, uint8_t flags) const;

    /// Get the first leaf token in this subtree.
    Token getFirstToken() const;

    /// Get the last leaf token in this subtree.
    Token getLastToken() const;

    /// Get the source range of the node.
    SourceRange sourceRange() const;

    /// Gets the child syntax node at the specified index. If the child at
    /// the given index is not a node (probably a token) then this returns null.
    const SyntaxNode* childNode(uint32_t index) const;
    Token childToken(uint32_t index) const;
    uint32_t getChildCount() const { return childCount; }

    template<typename T>
    T& as() {
        // TODO: assert kind
        return *static_cast<T*>(this);
    }

    template<typename T>
    const T& as() const {
        // TODO: assert kind
        return *static_cast<const T*>(this);
    }

    // The following is some template magic to determine whether a type has a
    // visit() function taking a specific argument, and if so call it. Otherwise
    // it calls visitDefault().
    template<typename, typename T>
    struct has_visit {
        static_assert(
            std::integral_constant<T, false>::value,
            "Second template parameter needs to be of function type.");
    };

    template<typename C, typename Ret, typename... Args>
    struct has_visit<C, Ret(Args...)> {
    private:
        template<typename T>
        static constexpr auto check(T*) -> typename
            std::is_same<
                decltype(std::declval<T>().visit(std::declval<Args>()...)),
                Ret
            >::type;

        template<typename>
        static constexpr std::false_type check(...);

        typedef decltype(check<C>(0)) type;

    public:
        static constexpr bool value = type::value;
    };

    template<typename C, typename... Args>
    static std::enable_if_t<has_visit<C, void(Args...)>::value> dispatch(C& c, Args&&... args) {
        c.visit(std::forward<Args>(args)...);
    }

    template<typename C, typename... Args>
    static std::enable_if_t<!has_visit<C, void(Args...)>::value> dispatch(C& c, Args&&... args) {
        c.visitDefault(std::forward<Args>(args)...);
    }

protected:
    uint32_t childCount = 0;

    virtual TokenOrSyntax getChild(uint32_t) const = 0;
};

template<typename T>
class SyntaxList : public SyntaxNode {
public:
    SyntaxList(std::nullptr_t) : SyntaxList(span<T* const>(nullptr)) {}
    SyntaxList(span<T* const> elements) :
        SyntaxNode(SyntaxKind::List),
        elements(elements)
    {
        childCount = (uint32_t)elements.size();
    }

    uint32_t count() const { return (uint32_t)elements.size(); }

    typename span<T* const>::const_iterator begin() const { return elements.begin(); }
    typename span<T* const>::const_iterator end() const { return elements.end(); }

    const T* operator[](uint32_t index) const { return elements[index]; }

private:
    TokenOrSyntax getChild(uint32_t index) const override final { return elements[index]; }

    span<T* const> elements;
};

class TokenList : public SyntaxNode {
public:
    TokenList(std::nullptr_t) : TokenList(span<Token const>(nullptr)) {}
    TokenList(span<Token const> elements) :
        SyntaxNode(SyntaxKind::List),
        elements(elements)
    {
        childCount = (uint32_t)elements.size();
    }

    uint32_t count() const { return (uint32_t)elements.size(); }

    span<Token const>::const_iterator begin() const { return elements.begin(); }
    span<Token const>::const_iterator end() const { return elements.end(); }

    Token operator[](uint32_t index) const { return elements[index]; }

private:
    TokenOrSyntax getChild(uint32_t index) const override final { return elements[index]; }

    span<Token const> elements;
};

template<typename T>
class SeparatedSyntaxList : public SyntaxNode {
public:
    class const_iterator {
    public:
        using difference_type = ptrdiff_t;
        using value_type = const T*;
        using pointer = const T**;
        using reference = const T*;
        using iterator_category = std::forward_iterator_tag;

        const_iterator(const SeparatedSyntaxList& list, int index) :
            list(list), index(index)
        {
        }

        const_iterator& operator++() { ++index; return *this; }
        const_iterator operator++(int) {
            const_iterator result = *this;
            ++(*this);
            return result;
        }

        reference operator*() const { return list[index]; }
        pointer operator->() const { return &list[index]; }

        bool operator==(const const_iterator& other) const { return &list == &other.list && index == other.index; }
        bool operator!=(const const_iterator& other) const { return !(*this == other); }

    private:
        const SeparatedSyntaxList& list;
        int index;
    };

    SeparatedSyntaxList(std::nullptr_t) : SeparatedSyntaxList(span<TokenOrSyntax const>(nullptr)) {}
    SeparatedSyntaxList(span<TokenOrSyntax const> elements) :
        SyntaxNode(SyntaxKind::List),
        elements(elements)
    {
        childCount = (uint32_t)elements.size();
    }

    bool empty() const { return count() == 0; }
    uint32_t count() const { return (uint32_t)std::ceil(elements.size() / 2.0); }

    const T* operator[](uint32_t index) const {
        index <<= 1;
        ASSERT(!elements[index].isToken);
        return static_cast<const T*>(elements[index].node);
    }

    const_iterator begin() const { return cbegin(); }
    const_iterator end() const { return cend(); }
    const_iterator cbegin() const { return const_iterator(*this, 0); }
    const_iterator cend() const { return const_iterator(*this, count()); }

private:
    TokenOrSyntax getChild(uint32_t index) const override final { return elements[index]; }

    span<TokenOrSyntax const> elements;
};

}
