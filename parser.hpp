#pragma once

extern int CurTok;

int getNextToken();

void HandleExtern();

void HandleDefinition();

void HandleExpression();