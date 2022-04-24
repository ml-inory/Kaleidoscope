#pragma once

static int CurTok;

int getNextToken();

void HandleExtern();

void HandleDefinition();

void HandleExpression();