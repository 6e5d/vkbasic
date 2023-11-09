#include <assert.h>
#include <stdlib.h>
#include <vulkan/vulkan.h>

#include "../include/pipeline.h"

VkbasicPipelineConf* vkbasic_pipeline_configure(
	VkShaderModule vert,
	VkShaderModule frag
) {
	VkStructureType e; // save big enum

	VkbasicPipelineConf* conf = malloc(sizeof(VkbasicPipelineConf));
	VkPipelineShaderStageCreateInfo ss_vert = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
		.stage = VK_SHADER_STAGE_VERTEX_BIT,
		.module = vert,
		.pName = "main",
	};

	VkPipelineShaderStageCreateInfo ss_frag = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
		.stage = VK_SHADER_STAGE_FRAGMENT_BIT,
		.module = frag,
		.pName = "main",
	};

	e = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	VkPipelineVertexInputStateCreateInfo vis = {
		.sType = e,
		.vertexBindingDescriptionCount = 0,
		.vertexAttributeDescriptionCount = 0,
	};

	e = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	VkPipelineInputAssemblyStateCreateInfo ias = {
		.sType = e,
		.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
		.primitiveRestartEnable = VK_FALSE,
	};

	VkViewport viewport = {
		.x = 0.0f,
		.y = 0.0f,
		.width = 900.0f,
		.height = 600.0f,
		.minDepth = 0.0f,
		.maxDepth = 1.0f,
	};

	VkRect2D scissor = {
		.offset = {0, 0},
		.extent = {99999, 99999},
	};

	VkPipelineViewportStateCreateInfo viewport_state = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
		.viewportCount = 1,
		.scissorCount = 1,
	};

	e = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	VkPipelineRasterizationStateCreateInfo rasterizer = {
		.sType = e,
		.depthClampEnable = VK_FALSE,
		.rasterizerDiscardEnable = VK_FALSE,
		.polygonMode = VK_POLYGON_MODE_FILL,
		.lineWidth = 1.0f,
		.cullMode = VK_CULL_MODE_BACK_BIT,
		.frontFace = VK_FRONT_FACE_CLOCKWISE,
		.depthBiasEnable = VK_FALSE,
	};

	e = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	VkPipelineMultisampleStateCreateInfo multisampling = {
		.sType = e,
		.sampleShadingEnable = VK_FALSE,
		.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
	};

	VkPipelineColorBlendAttachmentState cba = {
		.colorWriteMask =
			VK_COLOR_COMPONENT_R_BIT |
			VK_COLOR_COMPONENT_G_BIT |
			VK_COLOR_COMPONENT_B_BIT |
			VK_COLOR_COMPONENT_A_BIT,
		.blendEnable = VK_FALSE,
	};

	e = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	VkPipelineColorBlendStateCreateInfo cb = {
		.sType = e,
		.logicOpEnable = VK_FALSE,
		.logicOp = VK_LOGIC_OP_COPY,
		.attachmentCount = 1,
		.blendConstants[0] = 0.0f,
		.blendConstants[1] = 0.0f,
		.blendConstants[2] = 0.0f,
		.blendConstants[3] = 0.0f,
	};

	VkPipelineLayoutCreateInfo pl = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
		.setLayoutCount = 0,
		.pushConstantRangeCount = 0,
	};

	VkStructureType tmp =
		VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	VkPipelineDepthStencilStateCreateInfo depthstencil = {
		.sType = tmp,
		.depthTestEnable = VK_TRUE,
		.depthWriteEnable = VK_TRUE,
		.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL,
		.depthBoundsTestEnable = VK_FALSE,
		.back.failOp = VK_STENCIL_OP_KEEP,
		.back.passOp = VK_STENCIL_OP_KEEP,
		.back.compareOp = VK_COMPARE_OP_ALWAYS,
		.stencilTestEnable = VK_FALSE,
		.front.failOp = VK_STENCIL_OP_KEEP,
		.front.passOp = VK_STENCIL_OP_KEEP,
		.front.compareOp = VK_COMPARE_OP_ALWAYS,
	};

	*conf = (VkbasicPipelineConf) {
		.ss_vert = ss_vert,
		.ss_frag = ss_frag,
		.vis = vis,
		.ias = ias,
		.viewport = viewport,
		.scissor = scissor,
		.viewport_state = viewport_state,
		.rasterizer = rasterizer,
		.multisampling = multisampling,
		.depthstencil = depthstencil,
		.cba = cba,
		.cb = cb,
		.pl = pl,
	};
	conf->viewport_state.pViewports = &conf->viewport,
	conf->viewport_state.pScissors = &conf->scissor,
	conf->cb.pAttachments = &conf->cba;
	return conf;
}

void vkbasic_pipeline_standard(
	VkPipeline* pipeline,
	VkPipelineLayout* pipelinelayout,
	VkbasicPipelineConf* conf,
	VkRenderPass renderpass,
	VkDevice device
) {
	VkPipelineShaderStageCreateInfo stages[] =
		{conf->ss_vert, conf->ss_frag};
	assert(0 == vkCreatePipelineLayout(
		device, &conf->pl, NULL, pipelinelayout));
	VkGraphicsPipelineCreateInfo info = {
		.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
		.stageCount = 2,
		.pVertexInputState = &conf->vis,
		.pInputAssemblyState = &conf->ias,
		.pViewportState = &conf->viewport_state,
		.pRasterizationState = &conf->rasterizer,
		.pMultisampleState = &conf->multisampling,
		.pDepthStencilState = &conf->depthstencil,
		.pColorBlendState = &conf->cb,
		.pStages = stages,
		.layout = *pipelinelayout,
		.renderPass = renderpass,
		.subpass = 0,
		.basePipelineHandle = VK_NULL_HANDLE,
	};
	assert(0 == vkCreateGraphicsPipelines(
		device,
		VK_NULL_HANDLE,
		1,
		&info,
		NULL,
		pipeline
	));
}
