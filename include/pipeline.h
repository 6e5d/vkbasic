#include <vulkan/vulkan.h>

typedef struct {
	VkPipelineShaderStageCreateInfo ss_vert;
	VkPipelineShaderStageCreateInfo ss_frag;
	VkPipelineVertexInputStateCreateInfo vis;
	VkPipelineInputAssemblyStateCreateInfo ias;
	VkViewport viewport;
	VkRect2D scissor;
	VkPipelineViewportStateCreateInfo viewport_state;
	VkPipelineRasterizationStateCreateInfo rasterizer;
	VkPipelineMultisampleStateCreateInfo multisampling;
	VkPipelineDepthStencilStateCreateInfo depthstencil;
	VkPipelineColorBlendAttachmentState cba;
	VkPipelineColorBlendStateCreateInfo cb;
	VkPipelineLayoutCreateInfo pl;
} VkbasicPipelineConf;

VkbasicPipelineConf* vkbasic_pipeline_configure(
	VkShaderModule vert,
	VkShaderModule frag
);

void vkbasic_pipeline_standard(
	VkPipeline* pipeline,
	VkPipelineLayout* pipelinelayout,
	VkbasicPipelineConf* conf,
	VkRenderPass renderpass,
	VkDevice device
);
