/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 17:58:19 by plouvel           #+#    #+#             */
/*   Updated: 2022/04/22 12:51:59 by dhubleur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include "ft_dprintf.h"
#include "minishell.h"

/* gestions d'erreurs et simplification */

void	*parse_list(t_dlist *node, t_command *command, int *arg_count)
{
	t_arg_type	arg_type;

	arg_type = ((t_arg *) node->content)->type;
	if (arg_type == ARG_WORD)
		(*arg_count)++;
	else if (arg_type == ARG_REDIRECT_HERE_DOC)
	{
		if (command->here_doc > 0)
			close(command->here_doc);
		here_doc_logic(node->content, command);
	}
	else
	{
		if (!add_io(node->content, command))
			return (NULL);
	}
	if (node->next && !g_sigint)
	{
		if (!parse_list(node->next, command, arg_count))
			return (NULL);
	}
	return (node);
}

t_command	*check_error_cause(t_command *command, t_minishell *minishell)
{
	if (minishell->err != 0)
		return (NULL);
	return (command);
}

t_command	*prepare_command(bool piped, t_ast_tree_node *node,
	int *arg_count, t_minishell *minishell)
{
	t_command	*command;

	command = init_cmd();
	if (!command)
		return (NULL);
	if (node->args != NULL)
	{
		command->is_piped = piped;
		command->name = get_path_from_name(
				find_command_name(node->args), minishell, command);
		if (!command->name)
			return (check_error_cause(command, minishell));
		*arg_count = 0;
		if (node->args->next != NULL)
			parse_list(node->args, command, arg_count);
	}
	else
	{
		command->empty_command = 1;
		command->name = NULL;
	}
	return (command);
}

t_command	*parse_command(t_ast_tree_node *node, bool piped,
	t_minishell *minishell)
{
	t_command	*command;
	int			args_count;
	t_dlist		*elem;
	int			i;

	command = prepare_command(piped, node, &args_count, minishell);
	if (!command)
		return (NULL);
	if (command->name != NULL && !g_sigint)
	{
		command->args = ft_calloc(sizeof(char *), args_count + 2);
		if (!command->args)
			return (NULL);
		elem = node->args;
		i = 0;
		while (elem)
		{
			if (((t_arg *)elem->content)->type == ARG_WORD)
				command->args[i++] = ((t_arg *)elem->content)->value;
			elem = elem->next;
		}
		command->args[i] = NULL;
	}
	command->next = NULL;
	return (command);
}

t_command	*parse_commands(t_ast_tree_node *root, t_minishell *minishell)
{
	t_command	*first;

	first = NULL;
	apply_expansion_on_node(root, minishell);
	if (!root && minishell->err != 0)
		return (NULL);
	if (root->type == NODE_COMMAND)
	{
		first = parse_command(root, false, minishell);
		if (!first)
			display_error_more(NULL, "malloc", 0);
	}
	else
	{
		while (root->type == NODE_PIPE && !g_sigint)
		{
			if (!test_parse_and_add(root->left, minishell, &first, true))
				return (NULL);
			root = root->right;
		}
		if (!g_sigint)
			if (!test_parse_and_add(root, minishell, &first, false))
				return (NULL);
	}
	return (first);
}
